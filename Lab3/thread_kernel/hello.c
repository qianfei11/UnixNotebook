#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/thread_info.h>
#include <linux/kthread.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("assassinq");
MODULE_DESCRIPTION("hello world module");

struct task_struct *thread = NULL;

int func(void *data) {
    int i = 0;
    while (!kthread_should_stop()) {
        printk("Kernel Thread wakes up: %d\n", i++);
        ssleep(1);
    }
    return 0;
}

static int __init hello_init(void) {
    printk("Hello World!\n");
    thread = kthread_run(func, 0, "kernel_thread");
    return 0;
}

static void __exit hello_exit(void) {
    printk("Hello exit!\n");
    kthread_stop(thread);
}

module_init(hello_init);
module_exit(hello_exit);
