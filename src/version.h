// Copyright (c) 2012-2018 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2018 EXOSIS developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_VERSION_H
#define BITCOIN_VERSION_H

/**
 * network protocol versioning
 */

//static const int PROTOCOL_VERSION = 70015;
// EXOSIS BEGIN
//static const int PROTOCOL_VERSION = 70208;
static const int PROTOCOL_VERSION = 85019;
// EXOSIS END

//! initial proto version, to be increased after version/verack negotiation
static const int INIT_PROTO_VERSION = 209;

//! In this version, 'getheaders' was introduced.
//static const int GETHEADERS_VERSION = 31800;
// EXOSIS BEGIN
//static const int GETHEADERS_VERSION = 70077;
static const int GETHEADERS_VERSION = 80008;
// EXOSIS END

//! disconnect from peers older than this proto version
// EXOSIS BEGIN
//static const int MIN_PEER_PROTO_VERSION = GETHEADERS_VERSION;
static const int MIN_PEER_PROTO_VERSION = 85019;
// EXOSIS END

//! nTime field added to CAddress, starting with this version;
//! if possible, avoid requesting addresses nodes older than this
static const int CADDR_TIME_VERSION = 31402;

//! BIP 0031, pong message, is enabled for all versions AFTER this one
static const int BIP0031_VERSION = 60000;

//! "filter*" commands are disabled without NODE_BLOOM after and including this version
//static const int NO_BLOOM_VERSION = 70011;
static const int NO_BLOOM_VERSION = 70201;

//! "sendheaders" command and announcing blocks with headers starts with this version
//static const int SENDHEADERS_VERSION = 70012;
static const int SENDHEADERS_VERSION = 70201;

//! "feefilter" tells peers to filter invs to you by fee starts with this version
static const int FEEFILTER_VERSION = 70013;

//! short-id-based block download starts with this version
static const int SHORT_IDS_BLOCKS_VERSION = 70014;

//! not banning for invalid compact blocks starts with this version
static const int INVALID_CB_NO_BAN_VERSION = 70015;

//! DIP0001 was activated in this version
static const int DIP0001_PROTOCOL_VERSION = 70208;

#endif // BITCOIN_VERSION_H
