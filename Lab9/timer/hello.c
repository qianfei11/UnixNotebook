#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("assassinq");
MODULE_DESCRIPTION("hello world module");

static struct timer_list my_timer; // 创建定时器
static unsigned long start_jiffies; // 用于存储当前jiffies

void my_timer_func(struct timer_list *t) {
	unsigned long elapsed_time = jiffies - start_jiffies;
	start_jiffies = jiffies;
	my_timer.expires = start_jiffies + (5 * HZ); // 设置为当前jiffies加上5秒
	timer_setup(&my_timer, my_timer_func, 0); // 绑定定时器和函数
	add_timer(&my_timer); //
	printk("Elapsed time = %ld, %d\n", elapsed_time, elapsed_time / HZ);
}

static int __init hello_init(void) {
	printk(KERN_WARNING "hello world.\n");
	start_jiffies = jiffies;
	my_timer.expires = start_jiffies + (5 * HZ);
	timer_setup(&my_timer, my_timer_func, 0);
	add_timer(&my_timer);
	return 0;
}

static void __exit hello_exit(void) {
	del_timer(&my_timer); // 删除定时器
	printk(KERN_WARNING "hello exit!\n");
}

module_init(hello_init);
module_exit(hello_exit);
