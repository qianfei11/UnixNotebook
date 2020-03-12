#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/rbtree.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("assassinq");
MODULE_DESCRIPTION("kernel rbtree test module");

struct dog { // 创建dog结构体
	int weight;
	struct rb_node node; // 内嵌rb_node结构体
};

struct rb_root root = RB_ROOT; // 初始化红黑树

int insert_node(struct rb_root *root, struct dog *p) // 插入红黑树节点
{
    struct rb_node **link = &(root->rb_node), *parent = NULL;
    while (*link) { // 如果link下有儿子
        struct dog *f = rb_entry(*link, struct dog, node); // 获取节点对应的dog结构体
        int result = p->weight - f->weight;
        parent = *link;
        if (result < 0) // 如果小于该节点，往左子树走
            link = &((*link)->rb_left);
        else if (result > 0) // 如果大于该节点，往右子树走
            link = &((*link)->rb_right);
        else // 如果相等报错
            return false;
    }
    rb_link_node(&p->node, parent, link); // 初始化节点
    rb_insert_color(&p->node, root); // 插入节点
    return true;
}

struct dog *search_node(struct rb_root *root, int weight) // 搜索红黑树节点
{
    struct rb_node *node = root->rb_node;
    while (node) {
        struct dog *f = rb_entry(node, struct dog, node); // 获取节点对应的dog结构体
		int result = weight - f->weight;
        if (result < 0) // 如果小于该节点，往左子树搜索
            node = node->rb_left;
        else if (result > 0) // 如果大于该节点，往右子树搜索
            node = node->rb_right;
        else // 如果相等直接返回对应的dog结构体
            return f;
    }
    return NULL;
}

static int __init hello_init(void) {
	int i;
	struct dog *f;
	struct rb_node *pnode;

	for (i = 0; i < 10; i++) {
		f = (struct dog *) kmalloc(sizeof(*f), GFP_KERNEL); // 正常分配dog结构体对应的内存
		f->weight = i + 10;
		insert_node(&root, f); // 插入节点
	}

	for (pnode = rb_first(&root); pnode; pnode = rb_next(pnode)) { // 中序遍历红黑树
		printk("Dog weight = %d.\n", rb_entry(pnode, struct dog, node)->weight);
	}

	f = search_node(&root, 11); // 搜索指定的节点
	printk("Find a dog has weight = %d.\n", f->weight);
	return 0;
}

static void __exit hello_exit(void) {
	int i;
	struct dog *f;

	for(i = 0; i < 10; i++) {
		f = search_node(&root, i + 10); // 搜索指定的节点
		if (f) {
			rb_erase(&f->node, &root); // 删除节点
			printk("Free dog which has weight = %d.\n", f->weight);
			kfree(f); // 释放dog结构体内存
		}
	}
}

module_init(hello_init);
module_exit(hello_exit);
