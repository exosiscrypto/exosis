// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2018-2019 FXTC developers
// Copyright (c) 2019 EXOSIS developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <versionbitsinfo.h>

#include <assert.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "EXOSIS";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 100000;

        // EXOSIS BEGIN
        consensus.nMinimumSubsidy = 0.00100000 * COIN;
        // EXOSIS END

        // Dash
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMasternodePaymentsStartBlock = 10;
        consensus.nMasternodePaymentsIncreaseBlock = 50;
        consensus.nMasternodePaymentsIncreasePeriod = 365 * 1440; // 1 common year
        consensus.nMasternodeCollateralMinimum = 100000; // starting MN collateral
        consensus.nMasternodeCollateralMaximum = 100000; // MN collateral at infinity

        consensus.nInstantSendKeepLock = 24;

        consensus.nBudgetPaymentsStartBlock = 999 * 365 * 576; // 999 common years
        consensus.nBudgetPaymentsCycleBlocks = 10958; // weekly
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 86400; // 1 day

        consensus.nSuperblockStartBlock = 999 * 365 * 1440; // 999 common years
        consensus.nSuperblockCycle = 10958; // weekly

        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        //

        // EXOSIS TODO: BIP16Exception is valid for BTC blockchain only
        consensus.BIP16Exception = uint256S("0x00");
        consensus.BIP34Height = 1; // block 1
        consensus.BIP34Hash = uint256S("0x0000024d1a79e05cc2cbd754e63f4368e564f9215080e37190c53093cf9b6656"); // block
        consensus.BIP65Height = 25; // block
        consensus.BIP66Height = 25; // block
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 30 * 60; // 30 minutes
        consensus.nPowTargetSpacing = 2.5 * 60; // 2.5 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000d8867d4d6cbc0f"); // block 97610

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000000000b42cb61410d57be1d7e97553960f7957907a4eb37d923977d4a787"); // block 97610

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x36;
        pchMessageStart[1] = 0xc2;
        pchMessageStart[2] = 0xa1;
        pchMessageStart[3] = 0xee;
        nDefaultPort = 12330;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 1;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlock(1540402662, 2477745, 0x1e0ffff0, 1, 10 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000b6f3cad68583f2391b63af25851a200cd2973fb08812f881a4048e85a4e"));
        assert(genesis.hashMerkleRoot == uint256S("0x77c074dff95c472dc08ca31132e213660d4b996e0003870b88f3911460b50c8a"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("sync.exosis.io");
        vSeeds.emplace_back("sync1.exosis.io");

        // EXOSIS prefix 'f'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,33);
        // EXOSIS prefix 'F'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,4);
        // EXOSIS prefix 'X'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,111);
        // BIP32 prefixes
        base58Prefixes[EXT_PUBLIC_KEY] = {0xA6, 0x1C, 0x80, 0xF3};
        base58Prefixes[EXT_SECRET_KEY] = {0xA5, 0x2B, 0x41, 0x4D};

        bech32_hrp = "exosis";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        // Dash
        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "04c986c02f6b0a7683c73083cc31f3a0e47b3f2ab71e50747a599956d4be868d3f3762d63269bc1ff16896413c441b9c0273042f0f1d0512c4bae7145e0dfbde68";
        //

        // EXOSIS BEGIN
        founderAddress = "";
        // EXOSIS END

        checkpointData = {
            {
                {     0, uint256S("0x00000b6f3cad68583f2391b63af25851a200cd2973fb08812f881a4048e85a4e")},
                { 16261, uint256S("0x0000000000b154f7cf9e96e5b4db7d477ab11499d6d2b9776d05a5a02c0cc66e")},
                { 97610, uint256S("0x0000000000b42cb61410d57be1d7e97553960f7957907a4eb37d923977d4a787")},
            }
        };
        // EXOSIS BEGIN
        consensus.nlastValidPowHashHeight = 97610;
        // EXOSIS

        chainTxData = ChainTxData{
            // Data as of block 000005bdaeb941c7b686f7f6a03e8cf625a8d59162636a3af16fb46b44c9fc4a (height 5).
            1549060755, // * UNIX timestamp of last known number of transactions
            17130,          // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0           // * estimated number of transactions per second after that timestamp
        };

        // EXOSIS TODO: we need to resolve fee calculation bug and disable fallback
        ///* disable fallback fee on mainnet */
        //m_fallback_fee_enabled = false;
        m_fallback_fee_enabled = true;
        //
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 100000;

        // EXOSIS BEGIN
        consensus.nMinimumSubsidy = 0.00100000 * COIN;
        // EXOSIS END

        // Dash
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMasternodePaymentsStartBlock = 10;
        consensus.nMasternodePaymentsIncreaseBlock = 50;
        consensus.nMasternodePaymentsIncreasePeriod = 365 * 1440; // 1 common year
        consensus.nMasternodeCollateralMinimum = 100000; // starting MN collateral
        consensus.nMasternodeCollateralMaximum = 100000; // MN collateral at infinity

        consensus.nInstantSendKeepLock = 24;

        consensus.nBudgetPaymentsStartBlock = 999 * 365 * 576; // 999 common years
        consensus.nBudgetPaymentsCycleBlocks = 10958; // weekly
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 86400; // 1 day

        consensus.nSuperblockStartBlock = 999 * 365 * 1440; // 999 common years
        consensus.nSuperblockCycle = 10958; // weekly

        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        //

        // EXOSIS TODO: BIP16Exception is valid for BTC blockchain only
        consensus.BIP16Exception = uint256S("0x00");
        consensus.BIP34Height = 1; // block 1
        consensus.BIP34Hash = uint256S("0x0000024d1a79e05cc2cbd754e63f4368e564f9215080e37190c53093cf9b6656"); // block
        consensus.BIP65Height = 25; // block
        consensus.BIP66Height = 25; // block
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 30 * 60; // 30 minutes
        consensus.nPowTargetSpacing = 2.5 * 60; // 2.5 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000100001"); // genesis

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00"); // genesis

        pchMessageStart[0] = 0x36;
        pchMessageStart[1] = 0xc2;
        pchMessageStart[2] = 0xa1;
        pchMessageStart[3] = 0xee;
        nDefaultPort = 11330;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 1;
        m_assumed_chain_state_size = 0;

        genesis = CreateGenesisBlock(1548944406, 463245, 0x1e0ffff0, 1, 10 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000a097ac93e9acb1a74e22ad7faab04b52358c536b305c11a9ed2d5bb7a66"));
        assert(genesis.hashMerkleRoot == uint256S("0x77c074dff95c472dc08ca31132e213660d4b996e0003870b88f3911460b50c8a"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("sync.exosis.io");
        vSeeds.emplace_back("sync1.exosis.io");

        // EXOSIS prefix 't'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,33);
        // EXOSIS prefix 'T'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,4);
        // EXOSIS prefix 'Y'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,111);
        // BIP32 prefixes
        base58Prefixes[EXT_PUBLIC_KEY] = {0xA6, 0x1C, 0x80, 0xF3};
        base58Prefixes[EXT_SECRET_KEY] = {0xA5, 0x2B, 0x41, 0x4D};

        bech32_hrp = "testnet1exosis";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        // Dash
        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "045a90437df3a7c093927f11bb1a41955c1ac94031ba49cb4e46cd813b6cfc61eb94a1680d08f32361075f52e1e1466d407a75e5dc623cadc8e75add6afa3fe5ee";
        //

        // EXOSIS BEGIN
        founderAddress = "";
        // EXOSIS END

        checkpointData = {
            {
                {0, uint256S("0x00000a097ac93e9acb1a74e22ad7faab04b52358c536b305c11a9ed2d5bb7a66")},
            }
        };
        // EXOSIS BEGIN
        consensus.nlastValidPowHashHeight = 1;
        // EXOSIS

        chainTxData = ChainTxData{
            // Data as of block 00000706ff7091032d187cf1c2515a5d7891b6797f414526ef8a277491117d08 (height 0)
            0,
            0,
            0
        };

        /* enable fallback fee on testnet */
        m_fallback_fee_enabled = true;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 100000;

        // EXOSIS BEGIN
        consensus.nMinimumSubsidy = 0.00100000 * COIN;
        // EXOSIS END

        // Dash
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMasternodePaymentsStartBlock = 10;
        consensus.nMasternodePaymentsIncreaseBlock = 50;
        consensus.nMasternodePaymentsIncreasePeriod = 365 * 1440; // 1 common year
        consensus.nMasternodeCollateralMinimum = 100000; // starting MN collateral
        consensus.nMasternodeCollateralMaximum = 100000; // MN collateral at infinity

        consensus.nInstantSendKeepLock = 24;

        consensus.nBudgetPaymentsStartBlock = 999 * 365 * 576; // 999 common years
        consensus.nBudgetPaymentsCycleBlocks = 10958; // weekly
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 86400; // 1 day

        consensus.nSuperblockStartBlock = 999 * 365 * 1440; // 999 common years
        consensus.nSuperblockCycle = 10958; // weekly

        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        //

        // EXOSIS TODO: BIP16Exception is valid for BTC blockchain only
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = -1; // BIP34 has activated on regtest (blocks v1 are rejected in tests)
        consensus.BIP34Hash = uint256S("0x00"); //genesis hash
        consensus.BIP65Height = -1; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = -1; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 30 * 60; // 30 minutes
        consensus.nPowTargetSpacing = 2.5 * 60; // 2.5 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 10330;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        UpdateVersionBitsParametersFromArgs(args);

        genesis = CreateGenesisBlock(1540000200, 1298963, 0x1e0ffff0, 1, 10 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000002d44dc77b8f4b1198a57f9e966974627c4def4caec930a28c0113e25470"));
        assert(genesis.hashMerkleRoot == uint256S("0x77c074dff95c472dc08ca31132e213660d4b996e0003870b88f3911460b50c8a"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        // Dash
        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        //

        // EXOSIS BEGIN
        founderAddress = "";
        // EXOSIS END

        checkpointData = {
            {
                {0, uint256S("0x000002d44dc77b8f4b1198a57f9e966974627c4def4caec930a28c0113e25470")},
            }
        };
        // EXOSIS BEGIN
        consensus.nlastValidPowHashHeight = 0;
        // EXOSIS

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        // Bitcoin defaults
        // EXOSIS prefix 'c'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,88);
        // EXOSIS prefix 'C'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,28);
        // EXOSIS prefix 'Z'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,221);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "regtest1exosis";

        /* enable fallback fee on regtest */
        m_fallback_fee_enabled = true;
    }

    /**
     * Allows modifying the Version Bits regtest parameters.
     */
    void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
    void UpdateVersionBitsParametersFromArgs(const ArgsManager& args);
};

void CRegTestParams::UpdateVersionBitsParametersFromArgs(const ArgsManager& args)
{
    if (!args.IsArgSet("-vbparams")) return;

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> vDeploymentParams;
        boost::split(vDeploymentParams, strDeployment, boost::is_any_of(":"));
        if (vDeploymentParams.size() != 3) {
            throw std::runtime_error("Version bits parameters malformed, expecting deployment:start:end");
        }
        int64_t nStartTime, nTimeout;
        if (!ParseInt64(vDeploymentParams[1], &nStartTime)) {
            throw std::runtime_error(strprintf("Invalid nStartTime (%s)", vDeploymentParams[1]));
        }
        if (!ParseInt64(vDeploymentParams[2], &nTimeout)) {
            throw std::runtime_error(strprintf("Invalid nTimeout (%s)", vDeploymentParams[2]));
        }
        bool found = false;
        for (int j=0; j < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; ++j) {
            if (vDeploymentParams[0] == VersionBitsDeploymentInfo[j].name) {
                UpdateVersionBitsParameters(Consensus::DeploymentPos(j), nStartTime, nTimeout);
                found = true;
                LogPrintf("Setting version bits activation parameters for %s to start=%ld, timeout=%ld\n", vDeploymentParams[0], nStartTime, nTimeout);
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(strprintf("Invalid deployment (%s)", vDeploymentParams[0]));
        }
    }
}

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams(gArgs));
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}
