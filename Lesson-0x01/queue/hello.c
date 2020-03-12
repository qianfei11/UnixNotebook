#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/kfifo.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("assassinq");
MODULE_DESCRIPTION("kernel queue test module");

struct dog { // 创建dog结构体
	int tail_length;
	int weight;
};

struct kfifo fifo; // 创建队列

static int __init hello_init(void) {
	int i;
	int ret;
	struct dog *f;

	kfifo_alloc(&fifo, PAGE_SIZE, GFP_KERNEL); // 初始化队列
	if (ret) {
		return ret;
	}
	for (i = 0; i < 10; i++) {
		f = (struct dog *) kmalloc(sizeof(*f), GFP_KERNEL); // 正常分配dog结构体对应的内存
		f->tail_length = i + 6;
		f->weight = i + 10;
		kfifo_in(&fifo, &f, sizeof(*f)); // 将dog结构体入队列
		printk("[ENQUEUE] Dog tail_length = %d, weight = %d.\n", f->tail_length, f->weight);
	}

	ret = kfifo_out_peek(&fifo, &f, sizeof(*f)); // 查看指定位置的dog结构体
	if (ret != sizeof(*f)) {
		return -EINVAL;
	}
	printk("[PEEK] Dog has tail_length = %d, weight = %d.\n", f->tail_length, f->weight);
	return 0;
}

static void __exit hello_exit(void) {
	int ret;
	struct dog *f;

	while (kfifo_avail(&fifo)) { // 判断队列是否还有元素可取
		ret = kfifo_out(&fifo, &f, sizeof(*f)); // 将一个dog结构体出队列
		if (ret != sizeof(*f)) {
			return -EINVAL;
		}

		printk("[DEQUEUE] Free dog which has tail_length = %d, weight = %d.\n", f->tail_length, f->weight);
		kfree(f); // 释放出队列的dog结构体内存
	}
}

module_init(hello_init);
module_exit(hello_exit);
