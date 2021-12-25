/* adlist.h - A generic doubly linked list implementation
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ADLIST_H__
#define __ADLIST_H__

/* Node, List, and Iterator are the only data structures used currently. */

typedef struct listNode {
    // 前置节点指针
    struct listNode *prev;

    // 后置节点指针
    struct listNode *next;

    // 值的指针，void表示可以接受任意类型
    void *value;
} listNode;

typedef struct listIter {
    listNode *next;
    int direction;
} listIter;

typedef struct list {
    // 头节点指针
    listNode *head;

    // 尾节点指针
    listNode *tail;

    // 节点复制函数
    void *(*dup)(void *ptr);

    // 节点值释放函数
    void (*free)(void *ptr);

    // 节点值对比函数
    int (*match)(void *ptr, void *key);

    // 链表所包含的节点数量
    unsigned long len;
} list;

/* Functions implemented as macros */
#define listLength(l) ((l)->len)
#define listFirst(l) ((l)->head)
#define listLast(l) ((l)->tail)
#define listPrevNode(n) ((n)->prev)
#define listNextNode(n) ((n)->next)
#define listNodeValue(n) ((n)->value)

#define listSetDupMethod(l,m) ((l)->dup = (m))
#define listSetFreeMethod(l,m) ((l)->free = (m))
#define listSetMatchMethod(l,m) ((l)->match = (m))

#define listGetDupMethod(l) ((l)->dup)
#define listGetFreeMethod(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)

/* Prototypes */
// 创建一个空list
list *listCreate(void);

// 释放list
void listRelease(list *list);
// 将list置为空，但是会保留结构
void listEmpty(list *list);
// 从list头部插入值
list *listAddNodeHead(list *list, void *value);
// 从list尾部插入值
list *listAddNodeTail(list *list, void *value);
// 插入新的节点到指定节点后面或者前面，其中list表示链表；old_node表示目标节点；value表示新的值；after表示是将value插入到old_node的前面还是后面，0表示插入到前面，其他表示后面
list *listInsertNode(list *list, listNode *old_node, void *value, int after);
// 删除list中的node节点
void listDelNode(list *list, listNode *node);
// 获取list迭代器，direction表示迭代方式，0表示头，1表示尾
listIter *listGetIterator(list *list, int direction);
// 获取下一个节点
listNode *listNext(listIter *iter);
// 释放迭代器内存
void listReleaseIterator(listIter *iter);
// 拷贝一个list副本，内部通过头部迭代器方式实现
list *listDup(list *orig);
// 从list中搜索key的值，如果存在match（节点对比）函数，则使用match判断node的value是否相等，否则直接node.value == key判断
listNode *listSearchKey(list *list, void *key);
// 返回index位置的节点
listNode *listIndex(list *list, long index);
// 创建list的头部迭代器li
void listRewind(list *list, listIter *li);
// 创建list的尾部迭代器li
void listRewindTail(list *list, listIter *li);
// tail转移到head
void listRotateTailToHead(list *list);
// head转移到tail
void listRotateHeadToTail(list *list);
// 将o接到l上
void listJoin(list *l, list *o);

/* Directions for iterators */
#define AL_START_HEAD 0
#define AL_START_TAIL 1

#endif /* __ADLIST_H__ */
