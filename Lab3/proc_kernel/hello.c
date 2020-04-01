#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init_task.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("assassinq");
MODULE_DESCRIPTION("hello world module");

static int __init hello_init(void) {
    struct task_struct *me = current; // 获取当前进程
    struct task_struct *task;
    int i = 0;

    printk("Hello World!\n");
    printk("Current task: %s[%d].\n", me->comm, me->pid); // 输出当前进程
    for_each_process(task) { // 遍历进程
        printk("(%d) %s[%d].\n", i++, task->comm, task->pid);
    }
    printk("init_task: %s[%d].\n", init_task.comm, init_task.pid); // 输出init_task
    return 0;
}

static void __exit hello_exit(void) {
    printk("Hello exit!\n");
}

module_init(hello_init);
module_exit(hello_exit);
