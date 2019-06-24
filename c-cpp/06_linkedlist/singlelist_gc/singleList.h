#ifndef __SINGLELIST_H__
#define __SINGLELIST_H__

#include <stdlib.h>
#include <stdbool.h>
// 链表节点
typedef struct listNode
{
    struct listNode *next;
    void *value;
}listNode;
// 链表
typedef struct linkedList
{   // 链表头节点
    listNode *head;
    // 链表长度
    size_t len;
    size_t typesize;
    // 链表方法函数指针
    void(*dup)(void*, void*);
    int(*match)(void*, void*);
    void(*free)(void*);
}linkedList;

#define listSetDupMethod(l,m) ((l)->dup = (m))
#define listSetFreeMethod(l,m) ((l)->free = (m))
#define listSetMatchMethod(l,m) ((l)->match = (m))

#define listGetDupMethod(l) ((l)->dup)
#define listGetFree(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)
// 创建链表
linkedList *listCreate();
// 释放链表
void listRelease(linkedList *list);
// 清空链表内的节点
void listEmpty(linkedList *list);
// 链表头部插入 新节点
linkedList *listAddNodeHead(linkedList *list, void *value);

/**
 链表尾部插入 新节点

 @param list 链表
 @param value 新节点 value 值
 @return 链表
 */
linkedList *listAddNodeTail(linkedList *list, void *value);

/**
 链表中插入新 节点

 @param list 链表
 @param old_node 要插入新节点的位置
 @param value 新节点的值
 @param after 是否插入 到 old_node 的后面
 @return 链表
 */
linkedList *listInsertNode(linkedList *list, listNode *old_node, void *value, bool after);
// 链表删除节点
void listDelNode(linkedList *list, listNode *node);
// 查找 value = key 的节点
listNode *listSearchKey(linkedList *list, void *key);
// 查找第 index 个节点
listNode *listIndex(linkedList *list, long index);
// 反转链表
linkedList* listRewind(linkedList *list);
// 链表长度
size_t listLength(linkedList *list);

#endif // !__SINGLELIST_H__
