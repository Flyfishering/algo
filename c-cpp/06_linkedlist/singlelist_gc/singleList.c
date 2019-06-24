#include "singleList.h"

#include <string.h>
// 创建链表
linkedList * listCreate()
{
    linkedList *list = NULL;
    list = malloc(sizeof(*list));
    if (NULL == list)
    {
        return NULL;
    }
    
    list->dup = NULL;
    list->free = NULL;
    list->match = NULL;
    
    list->head = NULL;
    list->len = 0;
    
    return list;
}

// 释放链表
void listRelease(linkedList *list)
{
    if (NULL == list)
    {
        return;
    }
    
    listEmpty(list);
    
    free(list);
    list = NULL;
}
// 清空链表内的节点
void listEmpty(linkedList *list)
{
    if (NULL == list)
    {
        return;
    }
    
    while (NULL != list->head)
    {
        listNode *pNode = list->head;
        list->head = pNode->next;
        if (NULL != list->free)
        {
            list->free(pNode->value);
        }
        else
        {
            free(pNode->value);
        }
        
        pNode->next = NULL;
        free(pNode);
        pNode = NULL;
    }
}
// 链表头部插入 新节点
linkedList * listAddNodeHead(linkedList *list, void * value)
{
    if (NULL == list || NULL == value)
    {
        return list;
    }
    
    listNode *node = NULL;
    node = malloc(sizeof(*node));
    if (NULL == node)
    {
        return list;
    }
    
    node->value = value;
    node->next = list->head;
    list->head = node;
    
    ++list->len;
    return list;
}
// 链表尾部插入 新节点
linkedList * listAddNodeTail(linkedList *list, void *value)
{
    if (NULL == list || NULL == value)
    {
        return list;
    }
    
    listNode *node = NULL;
    node = malloc(sizeof(*node));
    if (NULL == node)
    {
        return list;
    }
    
    node->value = value;
    node->next = NULL;
    
    if (NULL == list->head
        && list->len == 0)
    {
        list->head = node;
    }
    else
    {
        listNode *tail = list->head;
        listNode *pre = list->head;
        while (NULL != tail)
        {
            pre = tail;
            tail = tail->next;
        }
        
        pre->next = node;
    }
    
    ++list->len;
    return list;
}
/**
 链表中插入新 节点
 
 @param list 链表
 @param old_node 要插入新节点的位置
 @param value 新节点的值
 @param after 是否插入 到 old_node 的后面
 @return 链表
 */
linkedList * listInsertNode(linkedList *list, listNode *old_node, void *value, bool after)
{
    // 判断参数是否合法
    if (NULL == list || NULL == old_node)
    {
        return list;
    }
    // 创建一个 节点 pNode 存放 value 值
    listNode *pNode = NULL;
    pNode = malloc(sizeof(*pNode));
    if (NULL == pNode)
    {
        return list;
    }
    pNode->value = value;
    // 判断是在 old_node 的前面还是后面插入 pNode
    if (after)
    {   // 后面插入
        pNode->next = old_node->next;
        old_node->next = pNode;
    }
    else
    {
        // 前面插入，因为是单向链表，只有从头遍历到 old_node 才能找到 old_node的前节点
        listNode *pre = list->head;
        while (pre->next != old_node)
        {
            pre = pre->next;
        }
        // 判断 pre 节点是否合法
        if (NULL != pre)
        {
            pre->next = pNode;
            pNode->next = old_node;
        }
    }
    
    ++list->len;
    return list;
}

// 删除节点
void listDelNode(linkedList *list, listNode *node)
{
    if (NULL == list || NULL == node)
    {
        return;
    }
    
    listNode *pre = list->head;
    listNode *cur = list->head;
    // 找到 node 节点
    while (NULL != cur && cur != node)
    {
        pre = cur;
        cur = cur->next;
    }
    
    // n没有找到 node 节点 则退出
    if (NULL == pre)
    {
        return;
    }
    // 删除 node 节点
    pre->next = node->next;
    node->next = NULL;
    // 链表大小 减一
    --list->len;
    // 释放 node 节点内存
    if (NULL != list->free)
    {
        list->free(node->value);
        free(node);
        node = NULL;
    }
}
// 查找 对应 value 的节点
listNode * listSearchKey(linkedList *list, void *key)
{
    // 链表不存在 退出
    if (NULL == list)
    {
        return NULL;
    }
    // 头结点
    listNode *node = list->head;
    // node 存在 则循环
    while (NULL != node)
    {
        // match 函数存在 则走 match 函数
        if (NULL != list->match)
        {
            if (list->match(key, node->value) == 0)
            {
                return node;
            }
        }
        else
        {
            // match 函数不存在 查找 key等于 node->value的节点
            if (key == node->value)
            {
                return node;
            }
        }
        // 下一个节点
        node = node->next;
    }
    
    return NULL;
}
// 查找节点对应的位置
listNode * listIndex(linkedList *list, long index)
{
    if (NULL == list)
    {
        return NULL;
    }
    // 是否非法 index
    if (index <= 0
        || index > list->len)
    {
        return NULL;
    }
    // 循环遍历，找到 index 对应的 节点
    listNode *pNode = list->head;
    for (long i = 0; i < index; ++i)
    {
        pNode = pNode->next;
    }
    
    return pNode;
}
// 反转链表
linkedList* listRewind(linkedList *list)
{
    if (NULL == list)
    {
        return NULL;
    }
    // 将头节点 指向 null 将下一个节点指向 头结点
    // 依次把后一个节点指向前一个节点
    // 将最后一个节点设为头节点
    listNode *head = list->head;
    listNode *pre = NULL;
    listNode *next = NULL;
    while (NULL != head)
    {
        // 找到下一个节点
        next = head->next;
        // 将该节点指向上一个节点
        head->next = pre;
        // 把 pre 设为 该节点
        pre = head;
        // 把 head 下一个节点 开启下一个循环 
        head = next;
    }
    // 设置最后一个节点为 头节点。
    list->head = pre;
    return list;
}
// 链表长度
size_t listLength(linkedList *list)
{
    if (NULL == list)
    {
        return 0;
    }
    
    return list->len;
}

