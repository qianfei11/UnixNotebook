#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("assassinq");
MODULE_DESCRIPTION("hello world module");

irq_handler_t key_handler(int irq, void *dev) {
    int scancode;
    int status;

    scancode = inb(0x60);
    status = inb(0x64);
    printk("Key pressed: scancode = 0x%x, status = 0x%x\n", scancode, status);

    return (irq_handler_t)IRQ_HANDLED;
}

static int __init hello_init(void) {
    int ret;

    ret = request_irq(1, (irq_handler_t)key_handler, IRQF_SHARED, "Key Hook", (void *)key_handler);
    if (ret) {
        printk("Request failed.\n");
        return -EIO;
    }
    printk("Keyboard Interrupt Hook.\n");

    return 0;
}

static void __exit hello_exit(void) {
    free_irq(1, (void *)key_handler);
    printk("Keyboard Interrupt Hook exited.\n");
}

module_init(hello_init);
module_exit(hello_exit);
