#include <stdio.h>
#include <stdbool.h>
// 链表 单元
struct single_list {
    // 指向下一个 链表单元的地址
	struct single_list *next;
	int val;
};
// 链表头地址
struct single_list_head {
    // 头地址
	struct single_list *head;
};

bool is_empty(struct single_list_head *head)
{
	return head->head == NULL;
}

void dump(struct single_list_head *head)
{
	struct single_list *tmp = head->head;
	int idx = 0;

	while (tmp) {
		printf("[%02d]: %08d\n", idx++, tmp->val);
		tmp = tmp->next;
	}
}

/**
 插入链表元素，将 elem 插入到 链表的第一个元素前面

 @param prev 链表的第一个元素
 @param elem 链表元素
 */
void insert(struct single_list **prev, struct single_list *elem)
{
	if (!prev)
		return;
    // elem 的 next 成员变量 指向链表头元素
	if (*prev)
		elem->next = *prev;
    // 将 elem 设置为 链表头元素
	*prev = elem;
}

/**
 链表插入元素

 @param head 链表的头元素地址
 @param elem 要插入的元素
 */
void insert_head(struct single_list_head *head, struct single_list *elem)
{   // &head->head 链表头元素  地址
	insert(&head->head, elem);
}

struct single_list* del(struct single_list **prev)
{
    // 声明一个临时变量
	struct single_list *tmp;

	if (!prev)
		return NULL;
    // 临时变量 赋值 为要删除的节点
	tmp = *prev;
    // 将要删除的节点指针指向 下一个节点的地址 即：*prev 节点被删除。
	*prev = (*prev)->next;
    // 这里的 tmp 就是之前要删除的 节点，现在把节点置空。
	tmp->next = NULL;

	return tmp;
};

struct single_list* delete_head(struct single_list_head* head)
{
	return del(&head->head);
};

struct single_list** search(struct single_list_head* head, int target)
{
    // prev 链表头指针地址
	struct single_list **prev, *tmp;
    // 遍历找到 val 不大于 target 的节点
	for (prev = &head->head, tmp = *prev;
	     tmp && (tmp->val < target);
	     prev = &tmp->next, tmp = *prev)
		;

	return prev;
};
// 翻转链表
void reverse(struct single_list_head* head)
{
	struct single_list_head tmp = {NULL};
	struct single_list *elem;

	while (!is_empty(head)) {
        // 删除头节点，并拿到这个被删除的头节点
		elem = delete_head(head);
        // 将 elem 插入新链表 tmp 中
		insert_head(&tmp, elem);
	}
    // 原链表头指针 指向 新链表的头部地址
	head->head = tmp.head;
}

bool is_cyclic(struct single_list_head* head)
{
	struct single_list *s1, *s2;

	s1 = s2 = head->head;
    // s1 遍历 一步
    // s2 遍历 两步
    // 如果出现 s1 == s2 这种情况，说明 s2 已经完整的走完了两遍链表，同时s1 刚刚好完整走完一遍链表 都是从头开始重新遍历，可以证明 该链表为循环链表
    // 如果没有出现上面这种情况，则说明 s2->next 存在 为 NULL 的情况。可以证明 该链表不是循环链表.
	while(s1 && s2) {
		s1 = s1->next;
		s2 = s2->next ? s2->next->next:s2->next;

		if (s1 == s2)
			return true;
	}
	return false;
}

struct single_list* middle(struct single_list_head* head)
{
    // 声明结构体指针
	struct single_list *s1, *s2;
    // 声明结构体变量
	struct single_list pseudo_head;
    // 赋值为 链表的头指针
	pseudo_head.next = head->head;
    // 赋值为 链表的头指针 地址
	s1 = s2 = &pseudo_head;
    // s1 遍历一步，s2 遍历两步，当 s2遍历到尾节点时，s1正好到最中间的节点。
	while (true) {
		if (!s2 || !s2->next)
			return s1;
		s1 = s1->next;
		s2 = s2->next->next;
	}

	return NULL;
};

int main()
{
    // 声明 链表头 结构体
	struct single_list_head head = {NULL};
    // 声明 链表单元 结构体数组
	struct single_list lists[10];
	struct single_list **prev;
	int idx;
    // 初始化 链表数组
	for (idx = 0; idx < 10; idx++) {
		lists[idx].val = idx;
		lists[idx].next = NULL;
	}
    // 链表中插入元素，插入顺序为 6,5,4,1,0, 链表中实际顺序为: 0->1->4->5->6
	insert_head(&head, &lists[6]);
	insert_head(&head, &lists[5]);
	insert_head(&head, &lists[4]);
	insert_head(&head, &lists[1]);
	insert_head(&head, &lists[0]);

	printf("=== insert 0, 1, 4, 5, 6\n");
    // 打印链表
	dump(&head);
    // 找到链表中值 val 不大于 2 的节点 
	prev = search(&head, 2);
    // 将 lists[2] 插入到 prev 节点后面
	insert(prev, &lists[2]);
	printf("=== insert 2\n");
    // 打印链表
	dump(&head);
    // 打印最中间的节点值
	printf("middle elem is %d\n", middle(&head)->val);
    
    // 判断链表中是否包含 2
	prev = search(&head, 2);
	if ((*prev) && ((*prev)->val == 2))
		printf("The list contains 2\n");
	else
		printf("The list not contains 2\n");
    // 删除 prev 节点
	del(prev);
    // 再次查找 链表中值 val 不大于 2 的节点 
	prev = search(&head, 2);
	printf("After remove 2\n");
    // 节点 2 已经被删除， 这时会打印 The list not contains 2
	if ((*prev) && ((*prev)->val == 2))
		printf("The list contains 2\n");
	else
		printf("The list not contains 2\n");
    // 打印整个 链表 会发现 节点 2  已经不存在链表中
	dump(&head);

	printf("After reverse \n");
    // 翻转链表
	reverse(&head);
    // 打印 翻转后的链表
	dump(&head);
    // 打印翻转后 链表的中间节点 值
	printf("middle elem is %d\n", middle(&head)->val);
    // 将单向链表 转变 为单向循环链表
	lists[0].next = &lists[6];
	printf("list is%s cyclic\n", is_cyclic(&head)?"":" not");

	return 0;
}
