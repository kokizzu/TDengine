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
#ifndef _TD_UTIL_LIST_H
#define _TD_UTIL_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

// Single linked list ================
#define TD_SLIST_NODE(TYPE) \
  struct {                  \
    struct TYPE *sl_next_;  \
  }

#define TD_SLIST(TYPE)      \
  struct {                  \
    struct TYPE *sl_head_;  \
    int          sl_neles_; \
  }

#define TD_SLIST_HEAD(sl) ((sl)->sl_head_)
#define TD_SLIST_NELES(sl) ((sl)->sl_neles_)
#define TD_SLIST_NODE_NEXT(sln) ((sln)->sl_next_)

#define TD_SLIST_INIT(sl)  \
  do {                     \
    (sl)->sl_head_ = NULL; \
    (sl)->sl_neles_ = 0;   \
  } while (0)

#define TD_SLIST_PUSH(sl, sln)                   \
  do {                                           \
    TD_SLIST_NODE_NEXT(sln) = TD_SLIST_HEAD(sl); \
    TD_SLIST_HEAD(sl) = (sln);                   \
    TD_SLIST_NELES(sl) += 1;                     \
  } while (0)

#define TD_SLIST_POP(sl)                                       \
  do {                                                         \
    TD_SLIST_HEAD(sl) = TD_SLIST_NODE_NEXT(TD_SLIST_HEAD(sl)); \
    TD_SLIST_NELES(sl) -= 1;                                   \
  } while (0)

// Double linked list ================
#define TD_DLIST_NODE(TYPE) \
  struct {                  \
    struct TYPE *dl_prev_;  \
    struct TYPE *dl_next_;  \
  }

#define TD_DLIST(TYPE)      \
  struct {                  \
    struct TYPE *dl_head_;  \
    struct TYPE *dl_tail_;  \
    int          dl_neles_; \
  }

#define TD_DLIST_NODE_PREV(dln) ((dln)->dl_prev_)
#define TD_DLIST_NODE_NEXT(dln) ((dln)->dl_next_)
#define TD_DLIST_HEAD(dl) ((dl)->dl_head_)
#define TD_DLIST_TAIL(dl) ((dl)->dl_tail_)
#define TD_DLIST_NELES(dl) ((dl)->dl_neles_)

#define TD_DLIST_INIT(dl)                         \
  do {                                            \
    TD_DLIST_HEAD(dl) = TD_DLIST_TAIL(dl) = NULL; \
    TD_DLIST_NELES(dl) = 0;                       \
  } while (0)

#define TD_DLIST_APPEND(dl, dln)                                \
  do {                                                          \
    if (TD_DLIST_HEAD(dl) == NULL) {                            \
      TD_DLIST_NODE_PREV(dln) = TD_DLIST_NODE_NEXT(dln) = NULL; \
      TD_DLIST_HEAD(dl) = TD_DLIST_TAIL(dl) = (dln);            \
    } else {                                                    \
      TD_DLIST_NODE_PREV(dln) = TD_DLIST_TAIL(dl);              \
      TD_DLIST_NODE_NEXT(dln) = NULL;                           \
      TD_DLIST_NODE_NEXT(TD_DLIST_TAIL(dl)) = (dln);            \
      TD_DLIST_TAIL(dl) = (dln);                                \
    }                                                           \
    TD_DLIST_NELES(dl) += 1;                                    \
  } while (0)

#define TD_DLIST_PREPEND(dl, dln)                               \
  do {                                                          \
    if (TD_DLIST_HEAD(dl) == NULL) {                            \
      TD_DLIST_NODE_PREV(dln) = TD_DLIST_NODE_NEXT(dln) = NULL; \
      TD_DLIST_HEAD(dl) = TD_DLIST_TAIL(dl) = (dln);            \
    } else {                                                    \
      TD_DLIST_NODE_PREV(dln) = NULL;                           \
      TD_DLIST_NODE_NEXT(dln) = TD_DLIST_HEAD(dl);              \
      TD_DLIST_NODE_PREV(TD_DLIST_HEAD(dl)) = (dln);            \
      TD_DLIST_HEAD(dl) = (dln);                                \
    }                                                           \
    TD_DLIST_NELES(dl) += 1;                                    \
  } while (0)

#define TD_DLIST_POP(dl, dln)                                                \
  do {                                                                       \
    if (TD_DLIST_HEAD(dl) == (dln)) {                                        \
      TD_DLIST_HEAD(dl) = TD_DLIST_NODE_NEXT(dln);                           \
    }                                                                        \
    if (TD_DLIST_TAIL(dl) == (dln)) {                                        \
      TD_DLIST_TAIL(dl) = TD_DLIST_NODE_PREV(dln);                           \
    }                                                                        \
    if (TD_DLIST_NODE_PREV(dln) != NULL) {                                   \
      TD_DLIST_NODE_NEXT(TD_DLIST_NODE_PREV(dln)) = TD_DLIST_NODE_NEXT(dln); \
    }                                                                        \
    if (TD_DLIST_NODE_NEXT(dln) != NULL) {                                   \
      TD_DLIST_NODE_PREV(TD_DLIST_NODE_NEXT(dln)) = TD_DLIST_NODE_PREV(dln); \
    }                                                                        \
    TD_DLIST_NELES(dl) -= 1;                                                 \
    TD_DLIST_NODE_PREV(dln) = TD_DLIST_NODE_NEXT(dln) = NULL;                \
  } while (0)

// General double linked list
typedef enum { TD_LIST_FORWARD, TD_LIST_BACKWARD } TD_LIST_DIRECTION_T;

typedef struct SListNode {
  TD_DLIST_NODE(SListNode);
  char data[];
} SListNode;

typedef struct {
  TD_DLIST(SListNode);
  int eleSize;
} SList;

typedef struct {
  SListNode *         next;
  TD_LIST_DIRECTION_T direction;
} SListIter;

#define listHead(l) TD_DLIST_HEAD(l)
#define listTail(l) TD_DLIST_TAIL(l)
#define listNEles(l) TD_DLIST_NELES(l)
#define listEleSize(l) ((l)->eleSize)
#define isListEmpty(l) (TD_DLIST_NELES(l) == 0)
#define listNodeFree(n) free(n)

void       tdListInit(SList *list, int eleSize);
void       tdListEmpty(SList *list);
SList *    tdListNew(int eleSize);
void *     tdListFree(SList *list);
void       tdListPrependNode(SList *list, SListNode *node);
void       tdListAppendNode(SList *list, SListNode *node);
int        tdListPrepend(SList *list, void *data);
int        tdListAppend(SList *list, void *data);
SListNode *tdListPopHead(SList *list);
SListNode *tdListPopTail(SList *list);
SListNode *tdListGetHead(SList *list);
SListNode *tsListGetTail(SList *list);
SListNode *tdListPopNode(SList *list, SListNode *node);
void       tdListMove(SList *src, SList *dst);
void       tdListDiscard(SList *list);

void       tdListNodeGetData(SList *list, SListNode *node, void *target);
void       tdListInitIter(SList *list, SListIter *pIter, TD_LIST_DIRECTION_T direction);
SListNode *tdListNext(SListIter *pIter);

#ifdef __cplusplus
}
#endif

#endif /*_TD_UTIL_LIST_H*/