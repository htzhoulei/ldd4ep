#include <linux/module.h>

static int num = 5;

/*
 * the perm argument specifies the permissions
 * of the corresponding file in sysfs
 */
/* S_IRUGO: everyone can read the sysfs entry */
module_param(num, int, S_IRUGO);

static int __init hello_init(void)
{
    pr_info("parameter num = %d.\n", num);
    return 0;
}

static void __exit hello_exit(void)
{
    pr_info("Hello world exit\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zhoulei");
MODULE_DESCRIPTION("This is a module that accepts parameters");