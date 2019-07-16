// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2018-2019 FXTC developers
// Copyright (c) 2019 EXOSIS developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DASH_NETFULFILLEDMAN_H
#define DASH_NETFULFILLEDMAN_H

#include <netbase.h>
#include <protocol.h>
#include <serialize.h>
#include <sync.h>

class CNetFulfilledRequestManager;
extern CNetFulfilledRequestManager netfulfilledman;

// Fulfilled requests are used to prevent nodes from asking for the same data on sync
// and from being banned for doing so too often.
class CNetFulfilledRequestManager
{
private:
    typedef std::map<std::string, int64_t> fulfilledreqmapentry_t;
    typedef std::map<CNetAddr, fulfilledreqmapentry_t> fulfilledreqmap_t;

    //keep track of what node has/was asked for and when
    fulfilledreqmap_t mapFulfilledRequests;
    CCriticalSection cs_mapFulfilledRequests;

public:
    CNetFulfilledRequestManager() {}

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        LOCK(cs_mapFulfilledRequests);
        READWRITE(mapFulfilledRequests);
    }

    void AddFulfilledRequest(CAddress addr, std::string strRequest); // expire after 1 hour by default
    bool HasFulfilledRequest(CAddress addr, std::string strRequest);
    void RemoveFulfilledRequest(CAddress addr, std::string strRequest);

    void CheckAndRemove();
    void Clear();

    std::string ToString() const;
};

#endif // DASH_NETFULFILLEDMAN_H
