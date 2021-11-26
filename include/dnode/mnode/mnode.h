/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TD_MNODE_H_
#define _TD_MNODE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------ TYPES EXPOSED ------------------------ */
typedef struct SDnode    SDnode;
typedef struct SMnode    SMnode;
typedef struct SMnodeMsg SMnodeMsg;
typedef void (*SendMsgToDnodeFp)(SDnode *pDnd, struct SEpSet *epSet, struct SRpcMsg *rpcMsg);
typedef void (*SendMsgToMnodeFp)(SDnode *pDnd, struct SRpcMsg *rpcMsg);
typedef void (*SendRedirectMsgFp)(SDnode *pDnd, struct SRpcMsg *rpcMsg);
typedef int32_t (*PutMsgToMnodeQFp)(SDnode *pDnd, SMnodeMsg *pMsg);

typedef struct SMnodeLoad {
  int64_t numOfDnode;
  int64_t numOfMnode;
  int64_t numOfVgroup;
  int64_t numOfDatabase;
  int64_t numOfSuperTable;
  int64_t numOfChildTable;
  int64_t numOfColumn;
  int64_t totalPoints;
  int64_t totalStorage;
  int64_t compStorage;
} SMnodeLoad;

typedef struct {
  int32_t           dnodeId;
  int64_t           clusterId;
  int8_t            replica;
  int8_t            selfIndex;
  SReplica          replicas[TSDB_MAX_REPLICA];
  struct SDnode    *pDnode;
  PutMsgToMnodeQFp  putMsgToApplyMsgFp;
  SendMsgToDnodeFp  sendMsgToDnodeFp;
  SendMsgToMnodeFp  sendMsgToMnodeFp;
  SendRedirectMsgFp sendRedirectMsgFp;
} SMnodeOptions;

/* ------------------------ SMnode ------------------------ */
/**
 * @brief Open a mnode.
 *
 * @param path Path of the mnode
 * @param pOptions Options of the mnode
 * @return SMnode* The mnode object
 */
SMnode *mnodeOpen(const char *path, const SMnodeOptions *pOptions);

/**
 * @brief Close a mnode
 *
 * @param pMnode The mnode object to close
 */
void mnodeClose(SMnode *pMnode);

/**
 * @brief Close a mnode
 *
 * @param pMnode The mnode object to close
 * @param pOptions Options of the mnode
 * @return int32_t 0 for success, -1 for failure
 */
int32_t mnodeAlter(SMnode *pMnode, const SMnodeOptions *pOptions);

/**
 * @brief Drop a mnode.
 *
 * @param path Path of the mnode.
 */
void mnodeDestroy(const char *path);

/**
 * @brief Get mnode statistics info
 *
 * @param pMnode The mnode object
 * @param pLoad Statistics of the mnode.
 * @return int32_t 0 for success, -1 for failure
 */
int32_t mnodeGetLoad(SMnode *pMnode, SMnodeLoad *pLoad);

/**
 * @brief Get user authentication info
 *
 * @param pMnode The mnode object
 * @param user
 * @param spi
 * @param encrypt
 * @param secret
 * @param ckey
 * @return int32_t 0 for success, -1 for failure
 */
int32_t mnodeRetriveAuth(SMnode *pMnode, char *user, char *spi, char *encrypt, char *secret, char *ckey);

/**
 * @brief Initialize mnode msg
 *
 * @param pMnode The mnode object
 * @param pMsg The request rpc msg
 * @return int32_t The created mnode msg
 */
SMnodeMsg *mnodeInitMsg(SMnode *pMnode, SRpcMsg *pRpcMsg);

/**
 * @brief Cleanup mnode msg
 *
 * @param pMsg The request msg
 */
void mnodeCleanupMsg(SMnodeMsg *pMsg);

/**
 * @brief Cleanup mnode msg
 *
 * @param pMsg The request msg
 * @param code The error code
 */
void mnodeSendRsp(SMnodeMsg *pMsg, int32_t code);

/**
 * @brief Process the read request
 *
 * @param pMnode The mnode object
 * @param pMsg The request msg
 * @return int32_t 0 for success, -1 for failure
 */
void mnodeProcessReadMsg(SMnode *pMnode, SMnodeMsg *pMsg);

/**
 * @brief Process the write request
 *
 * @param pMnode The mnode object
 * @param pMsg The request msg
 * @return int32_t 0 for success, -1 for failure
 */
void mnodeProcessWriteMsg(SMnode *pMnode, SMnodeMsg *pMsg);

/**
 * @brief Process the sync request
 *
 * @param pMnode The mnode object
 * @param pMsg The request msg
 * @return int32_t 0 for success, -1 for failure
 */
void mnodeProcessSyncMsg(SMnode *pMnode, SMnodeMsg *pMsg);

/**
 * @brief Process the apply request
 *
 * @param pMnode The mnode object
 * @param pMsg The request msg
 * @return int32_t 0 for success, -1 for failure
 */
void mnodeProcessApplyMsg(SMnode *pMnode, SMnodeMsg *pMsg);

#ifdef __cplusplus
}
#endif

#endif /*_TD_MNODE_H_*/
