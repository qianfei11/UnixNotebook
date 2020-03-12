#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/idr.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("assassinq");
MODULE_DESCRIPTION("kernel idr test module");

#define LOWER_LIMIT 1
#define UPPER_LIMIT 20

struct dog { // 创建dog结构体
	int tail_length;
	int weight;
};

struct idr idp; // 创建idr结构体

static int print_dog(int id, void *p, void *data) // 输出dog信息的函数
{
    struct dog *f = p;
	printk("[UID=%d] Dog tail_length = %d, weight = %d.\n", id, f->tail_length, f->weight);
    return 0;
}

static int __init hello_init(void) {
	int ret;
	int i;
	struct dog *f;

	idr_init(&idp); // 初始化idr结构体

	for (i = 0; i < 10; i++) {
		f = (struct dog *) kmalloc(sizeof(*f), GFP_KERNEL); // 正常分配dog结构体对应的内存
		f->tail_length = i + 6;
		f->weight = i + 10;

		idr_alloc(&idp, f, LOWER_LIMIT, UPPER_LIMIT, GFP_KERNEL); // 新创建一个idr映射，并自动分配uid
	}

	idr_for_each(&idp, print_dog, NULL); // 遍历idr中的元素，并输出dog信息
	return 0;
}

static int remove_dog(int id, void *p, void *data) { // 删除dog结构体函数
    struct dog *f = p;
	printk("[UID=%d] Free dog which has tail_length = %d, weight = %d.\n", id, f->tail_length, f->weight);
	kfree(f); // 释放dog结构体内存
    return 0;
}

static void __exit hello_exit(void) {
	int i;
	idr_for_each(&idp, remove_dog, NULL); // 遍历idr中的元素，并删除各个元素中dog结构体所对应的内存
	idr_destroy(&idp); // 删除idr结构体
}

module_init(hello_init);
module_exit(hello_exit);
