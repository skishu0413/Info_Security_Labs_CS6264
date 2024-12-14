#include <linux/module.h>   /* Needed by all kernel modules */
#include <linux/kernel.h>   /* Needed for loglevels (KERN_WARNING, KERN_EMERG, KERN_INFO, etc.) */
#include <linux/init.h>     /* Needed for __init and __exit macros. */
#include <linux/unistd.h>   /* sys_call_table __NR_* system call function indices */
#include <linux/fs.h>       /* filp_open */
#include <linux/slab.h>     /* kmalloc */
#include <linux/kallsyms.h> /* to find syscall table */
#include <asm/ptrace.h>     /* get pt_regs */

#include <asm/paravirt.h> /* write_cr0 */
#include <asm/uaccess.h>  /* get_fs, set_fs */

#include <linux/fcntl.h>
#include <linux/types.h>
#include <linux/namei.h>
#include <linux/security.h>
#include <linux/dcache.h>
#include <linux/path.h>

#include <linux/kprobes.h>
#include "hooks.h"

#define MAX_PATH_LEN   256

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YOUR NAME HERE");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");

// static struct kprobe kp_wait4 = {
// 	.symbol_name = "kernel_wait4"
// };
// static struct kprobe kp_waitid = {
// 	.symbol_name = "kernel_waitid"
// };
// static struct kprobe kp_fstat = {
// 	.symbol_name = "cp_new_stat"
// };
// static struct kprobe kp_fstatat = {
// 	.symbol_name = "cp_new_stat"
// };

// for later hooking
unsigned long *syscall_table = NULL;

// function pointer declarations for original syscall functions
asmlinkage long (*original_mkdir)(const struct pt_regs *);
asmlinkage long (*original_close)(const struct pt_regs *);
asmlinkage long (*original_creat)(const struct pt_regs *);
asmlinkage long (*original_open)(const struct pt_regs *);
asmlinkage long (*original_openat)(const struct pt_regs *);
asmlinkage long (*original_rename)(const struct pt_regs *);
asmlinkage long (*original_renameat)(const struct pt_regs *);
asmlinkage long (*original_brk)(const struct pt_regs *);
asmlinkage long (*original_mmap)(const struct pt_regs *);
asmlinkage long (*original_munmap)(const struct pt_regs *);
asmlinkage long (*original_mprotect)(const struct pt_regs *);
asmlinkage long (*original_clone)(const struct pt_regs *);
asmlinkage long (*original_fork)(const struct pt_regs *);
asmlinkage long (*original_execve)(const struct pt_regs *);
asmlinkage long (*original_exit)(const struct pt_regs *);
asmlinkage long (*original_exit_group)(const struct pt_regs *);
asmlinkage long (*original_read)(const struct pt_regs *);
asmlinkage long (*original_write)(const struct pt_regs *);
asmlinkage long (*original_fstat)(const struct pt_regs *);



// define what you want to do for each hook
asmlinkage long new_mkdir(const struct pt_regs *regs) {
    printk("[+] mkdir called");
     return original_mkdir(regs);
}

asmlinkage long new_close(const struct pt_regs *regs) {
    printk("[+] close called");
     return original_close(regs);
}

asmlinkage long new_creat(const struct pt_regs *regs) {
    printk("[+] create called");
     return original_creat(regs);
}

asmlinkage long new_open(const struct pt_regs *regs) {
    char filename[MAX_PATH_LEN];
    long copied = strncpy_from_user(filename, (char __user *)regs->di, sizeof(filename));
    if (copied > 0) {
        printk(KERN_ALERT "[ALERT] Malicious syscall: open on file: %s\n", filename);
    }
     return original_open(regs);
}

asmlinkage long new_openat(const struct pt_regs *regs) {
    printk("[+] openat called");
     return original_openat(regs);
}

asmlinkage long new_rename(const struct pt_regs *regs) {
    printk("[+] rename called");
     return original_rename(regs);
}

asmlinkage long new_renameat(const struct pt_regs *regs) {
    printk("[+] renameat called");
     return original_renameat(regs);
}

asmlinkage long new_brk(const struct pt_regs *regs) {
    printk("[+] brk called");
     return original_brk(regs);
}

asmlinkage long new_mmap(const struct pt_regs *regs) {
    printk("[+] mmap called");
     return original_mmap(regs);
}

asmlinkage long new_munmap(const struct pt_regs *regs) {
    printk("[+] munmap called");
     return original_munmap(regs);
}

asmlinkage long new_mprotect(const struct pt_regs *regs) {
    printk(KERN_ALERT "[ALERT] Malicious syscall: mprotect called\n");
     return original_mprotect(regs);
}

asmlinkage long new_clone(const struct pt_regs *regs) {
    printk("[+] clone called");
     return original_clone(regs);
}

asmlinkage long new_fork(const struct pt_regs *regs) {
    printk("[+] fork called");
     return original_fork(regs);
}

asmlinkage long new_execve(const struct pt_regs *regs) {
    char filename[MAX_PATH_LEN];
    long copied = strncpy_from_user(filename, (char __user *)regs->di, sizeof(filename));
    if (copied > 0) {
        printk(KERN_ALERT "[ALERT] Malicious syscall: execve on file: %s\n", filename);
    }
     return original_execve(regs);
}

asmlinkage long new_exit(const struct pt_regs *regs) {
    printk("[+] exit called");
     return original_exit(regs);
}

asmlinkage long new_exit_group(const struct pt_regs *regs) {
    printk("[+] exit_group called");
     return original_exit_group(regs);
}

asmlinkage long new_read(const struct pt_regs *regs) {
    printk("[+] read called");
     return original_read(regs);
}

asmlinkage long new_write(const struct pt_regs *regs) {
    char buf[64];
    unsigned long count = regs->dx;
    unsigned long copied = (count < sizeof(buf)) ? count : sizeof(buf) - 1;

    if (copy_from_user(buf, (char __user *)regs->si, copied) == 0) {
        buf[copied] = '\0';
        printk(KERN_ALERT "[ALERT] Malicious syscall: write to fd %ld, content: %s\n", regs->di, buf);
    }
     return original_write(regs);
}

asmlinkage long new_fstat(const struct pt_regs *regs) {
	printk("[+] fstat called");
	return original_fstat(regs);
}

inline void mywrite_cr0(unsigned long cr0) {
    asm volatile("mov %0,%%cr0" : "+r"(cr0), "+m"(__force_order));
}

void enable_write_protection(void) {
    unsigned long cr0 = read_cr0();
    set_bit(16, &cr0);
    mywrite_cr0(cr0);
}

void disable_write_protection(void) {
    unsigned long cr0 = read_cr0();
    clear_bit(16, &cr0);
    mywrite_cr0(cr0);
}

static int __init onload(void) {
    printk("Hello world!\n");

    syscall_table = (unsigned long *)kallsyms_lookup_name("sys_call_table");
  
    printk("Syscall table address: %p\n", syscall_table);
    printk("sizeof(unsigned long long *): %zx\n", sizeof(unsigned long long *));
    printk("sizeof(sys_call_table) : %zx\n", sizeof(syscall_table));
  
    if (syscall_table != NULL) {
        disable_write_protection(); // clear write protect
        
		original_mkdir = (void *)syscall_table[__NR_mkdir];
        syscall_table[__NR_mkdir] = (unsigned long long)&new_mkdir;
        original_close = (void *)syscall_table[__NR_close];
        syscall_table[__NR_close] = (unsigned long long)&new_close;
        original_creat = (void *)syscall_table[__NR_creat];
        syscall_table[__NR_creat] = (unsigned long long)&new_creat;
        original_open = (void *)syscall_table[__NR_open];
        syscall_table[__NR_open] = (unsigned long long)&new_open;
        original_openat = (void *)syscall_table[__NR_openat];
        syscall_table[__NR_openat] = (unsigned long long)&new_openat;
        original_rename = (void *)syscall_table[__NR_rename];
        syscall_table[__NR_rename] = (unsigned long long)&new_rename;
        original_renameat = (void *)syscall_table[__NR_renameat];
        syscall_table[__NR_renameat] = (unsigned long long)&new_renameat;
        original_brk = (void *)syscall_table[__NR_brk];
        syscall_table[__NR_brk] = (unsigned long long)&new_brk;
        original_mmap = (void *)syscall_table[__NR_mmap];
        syscall_table[__NR_mmap] = (unsigned long long)&new_mmap;
        original_munmap = (void *)syscall_table[__NR_munmap];
        syscall_table[__NR_munmap] = (unsigned long long)&new_munmap;
        original_mprotect = (void *)syscall_table[__NR_mprotect];
        syscall_table[__NR_mprotect] = (unsigned long long)&new_mprotect;
        original_clone = (void *)syscall_table[__NR_clone];
        syscall_table[__NR_clone] = (unsigned long long)&new_clone;
        original_fork = (void *)syscall_table[__NR_fork];
        syscall_table[__NR_fork] = (unsigned long long)&new_fork;
        original_execve = (void *)syscall_table[__NR_execve];
        syscall_table[__NR_execve] = (unsigned long long)&new_execve;
        original_exit = (void *)syscall_table[__NR_exit];
        syscall_table[__NR_exit] = (unsigned long long)&new_exit;
        original_exit_group = (void *)syscall_table[__NR_exit_group];
        syscall_table[__NR_exit_group] = (unsigned long long)&new_exit_group;
        original_write = (void *)syscall_table[__NR_write];
        syscall_table[__NR_write] = (unsigned long long)&new_write;
        original_read = (void *)syscall_table[__NR_read];
        syscall_table[__NR_read] = (unsigned long long)&new_read;
        original_fstat = (void *)syscall_table[__NR_fstat];
        syscall_table[__NR_fstat] = (unsigned long long)&new_fstat;

        enable_write_protection(); // reinstate write protect
    } else {
        printk("[-] onload: syscall_table is NULL\n");
    }
  
    /*
     * A non 0 return means init_module failed; module can't be loaded.
     */
    return 0;
}

static void __exit onunload(void) {
    if (syscall_table != NULL) {
        disable_write_protection();

        syscall_table[__NR_mkdir] = (unsigned long long)original_mkdir;
        syscall_table[__NR_close] = (unsigned long long)original_close;
        syscall_table[__NR_creat] = (unsigned long long)original_creat;
        syscall_table[__NR_open] = (unsigned long long)original_open;
        syscall_table[__NR_openat] = (unsigned long long)original_openat;
        syscall_table[__NR_rename] = (unsigned long long)original_rename;
        syscall_table[__NR_renameat] = (unsigned long long)original_renameat;
        syscall_table[__NR_brk] = (unsigned long long)original_brk;
        syscall_table[__NR_mmap] = (unsigned long long)original_mmap;
        syscall_table[__NR_munmap] = (unsigned long long)original_munmap;
        syscall_table[__NR_mprotect] = (unsigned long long)original_mprotect;
        syscall_table[__NR_clone] = (unsigned long long)original_clone;
        syscall_table[__NR_fork] = (unsigned long long)original_fork;
        syscall_table[__NR_execve] = (unsigned long long)original_execve;
        syscall_table[__NR_exit] = (unsigned long long)original_exit;
        syscall_table[__NR_exit_group] = (unsigned long long)original_exit_group;
        syscall_table[__NR_write] = (unsigned long long)original_write;
        syscall_table[__NR_read] = (unsigned long long)original_read;
        syscall_table[__NR_fstat] = (unsigned long long)original_fstat;

        enable_write_protection();
        printk("[+] onunload: sys_call_table unhooked\n");
    } else {
        printk("[-] onunload: syscall_table is NULL\n");
    }

    printk("Goodbye world!\n");
}

module_init(onload);
module_exit(onunload);
