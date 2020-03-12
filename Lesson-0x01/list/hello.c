#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("assassinq");
MODULE_DESCRIPTION("kernel list test module");

struct dog { // 创建dog结构体
	int tail_length;
	int weight;
	struct list_head list; // 将list_head嵌入到自建数据结构中
};

struct list_head dog_list; // 创建链表头

static int __init hello_init(void) {
	int i;
	struct dog *f; // 自建数据结构临时变量
	struct list_head *p; // 链表头临时变量

	printk("Here is a kernel list test.\n");
	INIT_LIST_HEAD(&dog_list); // 初始化链表头，将prev和next都指向自己

	for (i = 0; i < 10; i++) {
		f = (struct dog *) kmalloc(sizeof(*f), GFP_KERNEL); // 正常分配dog结构体对应的内存
		f->tail_length = i + 6;
		f->weight = i + 10;
		INIT_LIST_HEAD(&f->list); // 初始化dog结构体中的链表头

		list_add_tail(&f->list, &dog_list); // 将dog结构体的链表头添加到链表的尾部，并构成一个环形的双向链表
	}

	printk("Visit list with list_for_each().\n");
	list_for_each(p, &dog_list) { // 遍历链表，获取p节点
		f = list_entry(p, struct dog, list); // 获取p节点所对应的dog结构体，list指dog结构体中的链表头list项
		printk("Dog tail_length = %d, weight = %d.\n", f->tail_length, f->weight);
	}

	printk("Visit list with list_for_each_entry().\n");
	list_for_each_entry(f, &dog_list, list) { // 遍历链表，获取链表中节点所对应的dog结构体，list指dog结构体中的链表头list项
		printk("Dog tail_length = %d, weight = %d.\n", f->tail_length, f->weight);
	}
	return 0;
}

static void __exit hello_exit(void) {
	struct dog *f;
	struct list_head *p, *next;

	printk("Free all the dogs.\n");
	list_for_each_safe(p, next, &dog_list) { // 遍历链表，相比list_for_each多了一个存储next指针的缓存n，以防在删除节点后的pos的next指针指向其他地方
		f = list_entry(p, struct dog, list); // 获取p节点对应的dog结构体
		printk("Delete dog which has tail_length = %d, weight = %d.\n", f->tail_length, f->weight);
		list_del(p); // 在链表中删除对应的节点
		kfree(f); // 释放对应dog结构体的内存空间
	}
}

module_init(hello_init);
module_exit(hello_exit);
