#include <linux/module.h>

/* add header files to support character devices */
#include <linux/cdev.h>
#include <linux/fs.h>

/* 定义主设备号 */
#define MY_MAJOR_NUM 202

static struct cdev my_dev;

static int my_dev_open(struct inode *inode, struct file *file)
{
    pr_info("my_dev_open() is called.\n");
    return 0;
}

static int my_dev_close(struct inode *inode, struct file *file)
{
    pr_info("my_dev_close() is called.\n");
    return 0;
}

static long my_dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    pr_info("my_dev_ioctl() is called, cmd = %d, arg = %ld\n", cmd, arg);
    return 0;
}

/* 定义文件操作数据结构 */
static const struct file_operations my_dev_fops = {
    .owner = THIS_MODULE,
    .open = my_dev_open,
    .release = my_dev_close,
    .unlocked_ioctl = my_dev_ioctl,
};

static int __init hello_init(void)
{
    int ret;
    /* 获取设备ID */
    dev_t dev = MKDEV(MY_MAJOR_NUM, 0);
    pr_info("Hello world init\n");

    /* 分配设备号 */
    ret = register_chrdev_region(dev, 1, "my_char_device");
    if (ret < 0) {
        pr_info("Unable to allocate major munber %d\n", MY_MAJOR_NUM);
        return ret;
    }

    /* 初始化cdev数据结构，并添加到内核空间 */
    cdev_init(&my_dev, &my_dev_fops);
    ret = cdev_add(&my_dev, dev, 1);
    if (ret < 0) {
        unregister_chrdev_region(dev, 1);
        pr_info("Unable to add cdev\n");
        return ret;
    }
    return 0;
}

static void __exit hello_exit(void)
{
    pr_info("Hello world exit\n");
    cdev_del(&my_dev);
    unregister_chrdev_region(MKDEV(MY_MAJOR_NUM, 0), 1);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zhoulei");
MODULE_DESCRIPTION("This is a module that interacts with the ioctl system call");