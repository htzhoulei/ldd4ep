#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define DEVICE_NAME "mydev"
#define CLASS_NAME  "hello_class"

static struct class* helloClass;
static struct cdev my_dev;
dev_t dev;

static int my_dev_open(struct inode *inode, struct file *file)
{
    pr_info("my_dev_open() is called.\n");
    return 0;
}

static int my_dev_close(struct inode *inode, struct file *file)
{
    pr_info("my_dev_close() is called");
    return 0;
}

static long my_dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    pr_info("my_dev_ioctl() is called. cmd = %d, arg = %ld\n", cmd, arg);
    return 0;
}

/* 申明 file_opretions 数据结构 */
static const struct file_operations my_dev_fops = {
    .owner              = THIS_MODULE,
    .open               = my_dev_open,
    .release            = my_dev_close,
    .unlocked_ioctl     = my_dev_ioctl,
};

static int __init hello_init(void)
{
    int ret;
    dev_t dev_no;
    int Major;
    struct device* helloDevice;
    pr_info("Hello world init\n");

    /* 申请动态设备号 */
    ret = alloc_chrdev_region(&dev_no, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_info("Unable to allocate Major number \n");
        return ret;
    }

    /* 获取主设备号 */
    Major = MAJOR(dev_no);
    dev = MKDEV(Major, 0);

    pr_info("Allocated correctly with major number %d\n", Major);

    /* 初始化cdev数据结构，并将其添加到内核空间 */
    cdev_init(&my_dev, &my_dev_fops);
    ret = cdev_add(&my_dev, dev,1);
    if (ret < 0) {
        unregister_chrdev_region(dev, 1);
        pr_info("Unable to add cdev");
        return ret;
    }

    /* 注册设备类 */
    helloClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(helloClass)) {
        unregister_chrdev_region(dev, 1);
        cdev_del(&my_dev);
        pr_info("Failed to register device class\n");
        return PTR_ERR(helloClass);
    }
    pr_info("device class registered correctly\n");

    /* 创建名字为 DEVICE_NAME 的设备节点，并且关联设备号 */
    helloDevice = device_create(helloClass, NULL, dev, NULL, DEVICE_NAME);
    if (IS_ERR(helloDevice)) {
        class_destroy(helloClass);
        cdev_del(&my_dev);
        unregister_chrdev_region(dev, 1);
        pr_info("Failed to create the device\n");
        return PTR_ERR(helloDevice);
    }
    pr_info("The device is created correctly\n");

    return 0;
}

static void __exit hello_exit(void)
{
    device_destroy(helloClass, dev);    /* remove the device */
    class_destroy(helloClass);          /* remove the device class */
    cdev_del(&my_dev);
    unregister_chrdev_region(dev, 1);   /* inregister the device numbers */
    pr_info("Hello world wiht parameter exit\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zhoulei");
MODULE_DESCRIPTION("This is a module that interacts whis the ioctl system call");