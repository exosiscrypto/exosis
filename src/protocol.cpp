// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2018-2019 FXTC developers
// Copyright (c) 2019 EXOSIS developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <protocol.h>

#include <util/system.h>
#include <util/strencodings.h>

#ifndef WIN32
# include <arpa/inet.h>
#endif

static std::atomic<bool> g_initial_block_download_completed(false);

namespace NetMsgType {
const char *VERSION="version";
const char *VERACK="verack";
const char *ADDR="addr";
const char *INV="inv";
const char *GETDATA="getdata";
const char *MERKLEBLOCK="merkleblock";
const char *GETBLOCKS="getblocks";
const char *GETHEADERS="getheaders";
const char *TX="tx";
const char *HEADERS="headers";
const char *BLOCK="block";
const char *GETADDR="getaddr";
const char *MEMPOOL="mempool";
const char *PING="ping";
const char *PONG="pong";
const char *NOTFOUND="notfound";
const char *FILTERLOAD="filterload";
const char *FILTERADD="filteradd";
const char *FILTERCLEAR="filterclear";
const char *REJECT="reject";
const char *SENDHEADERS="sendheaders";
const char *FEEFILTER="feefilter";
const char *SENDCMPCT="sendcmpct";
const char *CMPCTBLOCK="cmpctblock";
const char *GETBLOCKTXN="getblocktxn";
const char *BLOCKTXN="blocktxn";
// Dash message types
const char *TXLOCKREQUEST="ix";
const char *TXLOCKVOTE="txlvote";
const char *SPORK="spork";
const char *GETSPORKS="getsporks";
const char *MASTERNODEPAYMENTVOTE="mnw";
const char *MASTERNODEPAYMENTBLOCK="mnwb";
const char *MASTERNODEPAYMENTSYNC="mnget";
const char *MNQUORUM="mn quorum"; // not implemented
const char *MNANNOUNCE="mnb";
const char *MNPING="mnp";
const char *DSACCEPT="dsa";
const char *DSVIN="dsi";
const char *DSFINALTX="dsf";
const char *DSSIGNFINALTX="dss";
const char *DSCOMPLETE="dsc";
const char *DSSTATUSUPDATE="dssu";
const char *DSTX="dstx";
const char *DSQUEUE="dsq";
const char *DSEG="dseg";
const char *SYNCSTATUSCOUNT="ssc";
const char *MNGOVERNANCESYNC="govsync";
const char *MNGOVERNANCEOBJECT="govobj";
const char *MNGOVERNANCEOBJECTVOTE="govobjvote";
const char *MNVERIFY="mnv";
} // namespace NetMsgType

/** All known message types. Keep this in the same order as the list of
 * messages above and in protocol.h.
 */
const static std::string allNetMessageTypes[] = {
    NetMsgType::VERSION,
    NetMsgType::VERACK,
    NetMsgType::ADDR,
    NetMsgType::INV,
    NetMsgType::GETDATA,
    NetMsgType::MERKLEBLOCK,
    NetMsgType::GETBLOCKS,
    NetMsgType::GETHEADERS,
    NetMsgType::TX,
    NetMsgType::HEADERS,
    NetMsgType::BLOCK,
    NetMsgType::GETADDR,
    NetMsgType::MEMPOOL,
    NetMsgType::PING,
    NetMsgType::PONG,
    NetMsgType::NOTFOUND,
    NetMsgType::FILTERLOAD,
    NetMsgType::FILTERADD,
    NetMsgType::FILTERCLEAR,
    NetMsgType::REJECT,
    NetMsgType::SENDHEADERS,
    NetMsgType::FEEFILTER,
    NetMsgType::SENDCMPCT,
    NetMsgType::CMPCTBLOCK,
    NetMsgType::GETBLOCKTXN,
    NetMsgType::BLOCKTXN,
    // Dash message types
    // NOTE: do NOT include non-implmented here, we want them to be "Unknown command" in ProcessMessage()
    NetMsgType::TXLOCKREQUEST,
    NetMsgType::TXLOCKVOTE,
    NetMsgType::SPORK,
    NetMsgType::GETSPORKS,
    NetMsgType::MASTERNODEPAYMENTVOTE,
    // NetMsgType::MASTERNODEPAYMENTBLOCK, // there is no message for this, only inventory
    NetMsgType::MASTERNODEPAYMENTSYNC,
    NetMsgType::MNANNOUNCE,
    NetMsgType::MNPING,
    NetMsgType::DSACCEPT,
    NetMsgType::DSVIN,
    NetMsgType::DSFINALTX,
    NetMsgType::DSSIGNFINALTX,
    NetMsgType::DSCOMPLETE,
    NetMsgType::DSSTATUSUPDATE,
    NetMsgType::DSTX,
    NetMsgType::DSQUEUE,
    NetMsgType::DSEG,
    NetMsgType::SYNCSTATUSCOUNT,
    NetMsgType::MNGOVERNANCESYNC,
    NetMsgType::MNGOVERNANCEOBJECT,
    NetMsgType::MNGOVERNANCEOBJECTVOTE,
    NetMsgType::MNVERIFY,
};
const static std::vector<std::string> allNetMessageTypesVec(allNetMessageTypes, allNetMessageTypes+ARRAYLEN(allNetMessageTypes));

CMessageHeader::CMessageHeader(const MessageStartChars& pchMessageStartIn)
{
    memcpy(pchMessageStart, pchMessageStartIn, MESSAGE_START_SIZE);
    memset(pchCommand, 0, sizeof(pchCommand));
    nMessageSize = -1;
    memset(pchChecksum, 0, CHECKSUM_SIZE);
}

CMessageHeader::CMessageHeader(const MessageStartChars& pchMessageStartIn, const char* pszCommand, unsigned int nMessageSizeIn)
{
    memcpy(pchMessageStart, pchMessageStartIn, MESSAGE_START_SIZE);
    memset(pchCommand, 0, sizeof(pchCommand));
    strncpy(pchCommand, pszCommand, COMMAND_SIZE);
    nMessageSize = nMessageSizeIn;
    memset(pchChecksum, 0, CHECKSUM_SIZE);
}

std::string CMessageHeader::GetCommand() const
{
    return std::string(pchCommand, pchCommand + strnlen(pchCommand, COMMAND_SIZE));
}

bool CMessageHeader::IsValid(const MessageStartChars& pchMessageStartIn) const
{
    // Check start string
    if (memcmp(pchMessageStart, pchMessageStartIn, MESSAGE_START_SIZE) != 0)
        return false;

    // Check the command string for errors
    for (const char* p1 = pchCommand; p1 < pchCommand + COMMAND_SIZE; p1++)
    {
        if (*p1 == 0)
        {
            // Must be all zeros after the first zero
            for (; p1 < pchCommand + COMMAND_SIZE; p1++)
                if (*p1 != 0)
                    return false;
        }
        else if (*p1 < ' ' || *p1 > 0x7E)
            return false;
    }

    // Message size
    if (nMessageSize > MAX_SIZE)
    {
        LogPrintf("CMessageHeader::IsValid(): (%s, %u bytes) nMessageSize > MAX_SIZE\n", GetCommand(), nMessageSize);
        return false;
    }

    return true;
}


ServiceFlags GetDesirableServiceFlags(ServiceFlags services) {
    if ((services & NODE_NETWORK_LIMITED) && g_initial_block_download_completed) {
        return ServiceFlags(NODE_NETWORK_LIMITED | NODE_WITNESS);
    }
    return ServiceFlags(NODE_NETWORK | NODE_WITNESS);
}

void SetServiceFlagsIBDCache(bool state) {
    g_initial_block_download_completed = state;
}


CAddress::CAddress() : CService()
{
    Init();
}

CAddress::CAddress(CService ipIn, ServiceFlags nServicesIn) : CService(ipIn)
{
    Init();
    nServices = nServicesIn;
}

void CAddress::Init()
{
    nServices = NODE_NONE;
    nTime = 100000000;
}

CInv::CInv()
{
    type = 0;
    hash.SetNull();
}

CInv::CInv(int typeIn, const uint256& hashIn) : type(typeIn), hash(hashIn) {}

bool operator<(const CInv& a, const CInv& b)
{
    return (a.type < b.type || (a.type == b.type && a.hash < b.hash));
}

std::string CInv::GetCommand() const
{
    std::string cmd;
    if (type & MSG_WITNESS_FLAG)
        cmd.append("witness-");
    int masked = type & MSG_TYPE_MASK;
    switch (masked)
    {
    case MSG_TX:             return cmd.append(NetMsgType::TX);
    case MSG_BLOCK:          return cmd.append(NetMsgType::BLOCK);
    case MSG_FILTERED_BLOCK: return cmd.append(NetMsgType::MERKLEBLOCK);
    case MSG_CMPCT_BLOCK:    return cmd.append(NetMsgType::CMPCTBLOCK);
    // Dash
    case MSG_TXLOCK_REQUEST:           return cmd.append(NetMsgType::TXLOCKREQUEST);
    case MSG_TXLOCK_VOTE:              return cmd.append(NetMsgType::TXLOCKVOTE);
    case MSG_SPORK:                    return cmd.append(NetMsgType::SPORK);
    case MSG_MASTERNODE_PAYMENT_VOTE:  return cmd.append(NetMsgType::MASTERNODEPAYMENTVOTE);
    case MSG_MASTERNODE_PAYMENT_BLOCK: return cmd.append(NetMsgType::MASTERNODEPAYMENTBLOCK);
    case MSG_MASTERNODE_QUORUM:        return cmd.append(NetMsgType::MNQUORUM);
    case MSG_MASTERNODE_ANNOUNCE:      return cmd.append(NetMsgType::MNANNOUNCE);
    case MSG_MASTERNODE_PING:          return cmd.append(NetMsgType::MNPING);
    case MSG_DSTX:                     return cmd.append(NetMsgType::DSTX);
    case MSG_GOVERNANCE_OBJECT:        return cmd.append(NetMsgType::MNGOVERNANCEOBJECT);
    case MSG_GOVERNANCE_OBJECT_VOTE:   return cmd.append(NetMsgType::MNGOVERNANCEOBJECTVOTE);
    case MSG_MASTERNODE_VERIFY:        return cmd.append(NetMsgType::MNVERIFY);
    //
    default:
        throw std::out_of_range(strprintf("CInv::GetCommand(): type=%d unknown type", type));
    }
}

std::string CInv::ToString() const
{
    try {
        return strprintf("%s %s", GetCommand(), hash.ToString());
    } catch(const std::out_of_range &) {
        return strprintf("0x%08x %s", type, hash.ToString());
    }
}

const std::vector<std::string> &getAllNetMessageTypes()
{
    return allNetMessageTypesVec;
}
