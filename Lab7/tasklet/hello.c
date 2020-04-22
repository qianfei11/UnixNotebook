#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("assassinq");
MODULE_DESCRIPTION("hello world module");

static unsigned char scancode, status;

DEFINE_SPINLOCK(key_lock); // spinlock

char *mappings[0x100];

void key_tasklet_func(unsigned long d) {
    unsigned char code;

    spin_lock(&key_lock);
    code = scancode;
    spin_unlock(&key_lock);

    if (code == 0xe0) {
        ;
    } else if (code & 0x80) { // release
        printk("In tasklet: released \"%s\"\n", mappings[code - 0x80]);
    } else { // press
        printk("In tasklet: pressed \"%s\"\n", mappings[code]);
    }
}

DECLARE_TASKLET(key_tasklet, key_tasklet_func, 0); // 初始化Tasklet
// DECLARE_TASKLET(key_tasklet_hi, key_tasklet_func, 0);

irq_handler_t key_handler(int irq, void *dev) {
    spin_lock(&key_lock);
    status = inb(0x64); // 读取键盘状态
    spin_unlock(&key_lock);

    spin_lock(&key_lock);
    scancode = inb(0x60); // 读取扫描码
    spin_unlock(&key_lock);

    printk("Key interrupt: scancode = 0x%x, status = 0x%x\n", scancode, status); // 输出对应按键的扫描码和状态
    tasklet_schedule(&key_tasklet); // 调度Tasklet
    // tasklet_hi_schedule(&key_tasklet_hi);

    return (irq_handler_t)IRQ_HANDLED;
}

void init_mappings(void) {
    memset(mappings, 0, sizeof(mappings));
    mappings[0x00] = "Error";
	mappings[0x01] = "Esc";
	mappings[0x02] = "1!";
	mappings[0x03] = "2@";
	mappings[0x04] = "3#";
	mappings[0x05] = "4$";
	mappings[0x06] = "5%";
	mappings[0x07] = "6^";
	mappings[0x08] = "7&";
	mappings[0x09] = "8*";
	mappings[0x0a] = "9(";
	mappings[0x0b] = "0)";
	mappings[0x0c] = "-_";
	mappings[0x0d] = "=+";
	mappings[0x0e] = "Backspace";
	mappings[0x0f] = "Tab";
	mappings[0x10] = "q";
	mappings[0x11] = "w";
	mappings[0x12] = "e";
	mappings[0x13] = "r";
	mappings[0x14] = "t";
	mappings[0x15] = "y";
	mappings[0x16] = "u";
	mappings[0x17] = "i";
	mappings[0x18] = "o";
	mappings[0x19] = "p";
	mappings[0x1a] = "[{";
	mappings[0x1b] = "]}";
	mappings[0x1c] = "Enter";
	mappings[0x1d] = "LCtrl";
	mappings[0x1e] = "a";
	mappings[0x1f] = "s";
	mappings[0x20] = "d";
	mappings[0x21] = "f";
	mappings[0x22] = "g";
	mappings[0x23] = "h";
	mappings[0x24] = "j";
	mappings[0x25] = "k";
	mappings[0x26] = "l";
	mappings[0x27] = ";:";
	mappings[0x28] = "'\"";
	mappings[0x29] = "`~";
	mappings[0x2c] = "z";
	mappings[0x2d] = "x";
	mappings[0x2e] = "c";
	mappings[0x2f] = "v";
	mappings[0x30] = "b";
	mappings[0x31] = "n";
	mappings[0x32] = "m";
	mappings[0x33] = ",<";
	mappings[0x34] = ".>";
	mappings[0x35] = "/?";
	mappings[0x36] = "RShift";
	mappings[0x37] = "Keypad-*/PrtScn";
	mappings[0x38] = "LAlt";
	mappings[0x39] = "Space bar";
	mappings[0x3a] = "CapsLock";
	mappings[0x3b] = "F1";
	mappings[0x3c] = "F2";
	mappings[0x3d] = "F3";
	mappings[0x3e] = "F4";
	mappings[0x3f] = "F5";
	mappings[0x40] = "F6";
	mappings[0x41] = "F7";
	mappings[0x42] = "F8";
	mappings[0x43] = "F9";
	mappings[0x44] = "F10";
	mappings[0x45] = "NumLock";
	mappings[0x46] = "ScrollLock";
	mappings[0x47] = "Keypad-7/Home";
	mappings[0x48] = "Keypad-8/Up";
	mappings[0x49] = "Keypad-9/PgUp";
	mappings[0x4a] = "Keypad--";
	mappings[0x4b] = "Keypad-4/Left";
	mappings[0x4c] = "Keypad-5";
	mappings[0x4d] = "Keypad-6/Right";
	mappings[0x4e] = "Keypad-+";
	mappings[0x4f] = "Keypad-1/End";
	mappings[0x50] = "Keypad-2/Down";
	mappings[0x51] = "Keypad-3/PgDn";
	mappings[0x52] = "Keypad-0/Ins";
	mappings[0x53] = "Keypad-./Del";
}

static int __init hello_init(void) {
    int ret;

    init_mappings();
    printk("Keyboard Interrupt Hook.\n");
    ret = request_irq(1, (irq_handler_t)key_handler, IRQF_SHARED, "Key Hook", (void *)key_handler); // 为中断服务例程分配一个硬件中断号
    if (ret) {
        printk("Request failed.\n");
        return -EIO;
    }

    return 0;
}

static void __exit hello_exit(void) {
    free_irq(1, (void *)key_handler); // 释放指定中断号
    printk("Keyboard Interrupt Hook exited.\n");
}

module_init(hello_init);
module_exit(hello_exit);
