// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2018-2019 The Exosis Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include "arith_uint256.h"
#include "chain.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"
#include <math.h>

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);

    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();
    if (params.fPowNoRetargeting || params.fPowAllowMinDifficultyBlocks)
    {
        return nProofOfWorkLimit;
    }

        // Genesis block
        if (pindexLast == NULL)
            return nProofOfWorkLimit;

        // Only change once per difficulty adjustment interval
        if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval(pindexLast->nHeight+1) != 0)
        {
            if (params.fPowAllowMinDifficultyBlocks)
            {
                // Special difficulty rule for testnet:
                // If the new block's timestamp is more than 2* 10 minutes
                // then allow mining of a min-difficulty block.
                if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.PowTargetSpacing(pindexLast->nHeight+1)*2)
                    return nProofOfWorkLimit;
                else
                {
                    // Return the last non-special-min-difficulty-rules-block
                    const CBlockIndex* pindex = pindexLast;
                    while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval(pindex->nHeight) != 0 && pindex->nBits == nProofOfWorkLimit)
                        pindex = pindex->pprev;
                    return pindex->nBits;
                }
            }
            //LogPrintf("difficulty adjustment interval %d  \n",(pindexLast->nHeight+1) % params.DifficultyAdjustmentIntervalV2());
            return pindexLast->nBits;
        }

        // Go back by what we want to be 14 days worth of blocks
        // Litecoin: This fixes an issue where a 51% attack can change difficulty at will.
        // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
        int blockstogoback2 = params.DifficultyAdjustmentInterval(pindexLast->nHeight+1)-1;
        if ((pindexLast->nHeight+1) != params.DifficultyAdjustmentInterval(pindexLast->nHeight+1))
            blockstogoback2 = params.DifficultyAdjustmentInterval(pindexLast->nHeight+1);

        // Go back by what we want to be 14 days worth of blocks
        const CBlockIndex* pindexFirst = pindexLast;
        for (int i = 0; pindexFirst && i < blockstogoback2; i++)
            pindexFirst = pindexFirst->pprev;

        assert(pindexFirst);
        return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    int fork2 = 21000;
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Initial //64_15 Written by Limx Dev 04/2017
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
	const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
	bool fShift;
	// Initial

    // fix wrong initial timespan value
    if (nActualTimespan < params.PowTargetTimespan(pindexLast->nHeight+1)/1.15)
        nActualTimespan = params.PowTargetTimespan(pindexLast->nHeight+1)/1.15;
    if (nActualTimespan > params.PowTargetTimespan(pindexLast->nHeight+1)*1.15)
        nActualTimespan = params.PowTargetTimespan(pindexLast->nHeight+1)*1.15;

    // Retarget
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    // BitCore: intermediate uint256 can overflow by 1 bit
    fShift = bnNew.bits() > bnPowLimit.bits() - 1;
    if (fShift)
        bnNew >>= 1;
    bnNew *= nActualTimespan;
    bnNew /= params.PowTargetTimespan(pindexLast->nHeight+1);
    if (fShift)
        bnNew <<= 1;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
