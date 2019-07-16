// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2018-2019 FXTC developers
// Copyright (c) 2019 EXOSIS developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <hash.h>
#include <tinyformat.h>
#include <util/strencodings.h>
#include <crypto/common.h>

// EXOSIS BEGIN
#include <crypto/scrypt.h> // Exosis Scrypt
#include <crypto/hashblock.h> // Exosis Timetravel
#include <crypto/x16r.h>
// EXOSIS END

uint256 CBlockHeader::GetHash() const
{
    // EXOSIS BEGIN
    //return SerializeHash(*this);
    return HashTimeTravel(BEGIN(nVersion), END(nNonce), GetBlockTime()); //TimeTravel
    // EXOSIS END
}

// EXOSIS BEGIN
uint256 CBlockHeader::GetPoWHash() const
{
    uint256 powHash = uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

    switch (nVersion & ALGO_VERSION_MASK)
    {
        case ALGO_EXOSIS:  powHash = GetHash(); break;
        case ALGO_X16R:    powHash = HashX16R(BEGIN(nVersion), END(nNonce), hashPrevBlock); break;
        default:           break; // EXOSIS TODO: we should not be here
    }

    return powHash;
}
// EXOSIS END

unsigned int CBlockHeader::GetAlgoEfficiency(int nBlockHeight) const
{
    switch (nVersion & ALGO_VERSION_MASK)
    {
        case ALGO_EXOSIS:  return       1;
        case ALGO_X16R:    return       1;
        default:           return       1; // EXOSIS TODO: we should not be here
    }

    return 1; // EXOSIS TODO: we should not be here
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, nMoneySupply=%s, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        // EXOSIS BEGIN
        //nTime, nBits, nNonce,
        nTime, nBits, nNonce, nMoneySupply,
        // EXOSIS END
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
