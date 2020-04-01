#include <linux/module.h> // 所有模块都需要的头文件
#include <linux/init.h>   // init&exit相关宏
#include <linux/kernel.h> // kernel中常见函数，如printk

MODULE_LICENSE("GPL"); // 声明开源许可证（GPL、BSD等等）
MODULE_AUTHOR("baoli"); // 声明作者
MODULE_DESCRIPTION("hello world module"); // 声明模块功能

static int __init hello_init(void)
{
      printk(KERN_WARNING "hello world.\n"); // 输出指定字符串
      return 0;
}
static void __exit hello_exit(void)
{
      printk(KERN_WARNING "hello exit!\n");
}

module_init(hello_init); // 设置入口函数
module_exit(hello_exit); // 设置出口函数
