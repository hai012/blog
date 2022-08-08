#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/types.h>
#include <linux/uaccess.h>





static int tun_open(struct inode *node, struct file *file)
{
	return 0;
}

static int tun_release(struct inode *node, struct file *file)
{
	return 0;
}

ssize_t tun_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{

    vnet_recv_packet();
    return 0;
}

ssize_t tun_read(struct file *, char __user *, size_t, loff_t *)
{
    vnet_free_wakeup
    return 0;
}



static struct file_operations tun_fops = {
    .owner = THIS_MODULE,
    .read  = tun_read,
    .write = tun_write,
    .open  = tun_open,
    .release = tun_release,
}


int char_device_init(void)
{
    //alloc_chrdev_region返回的int值等于0成功，小于0为错误编码
    if(alloc_chrdev_region(&tun_dev, MAJOR_BASE, MINOR_COUNT, "tun")){
        print_error("failed to alloc_chrdev_region\n");
        retun -1;
    }

    //cdev_alloc返回有效指针成功，返回NULL失败
    tun_cdev = cdev_alloc();
    if(NULL == tun_cdev){
        print_error("failed to alloc cdev\n");
        goto out_unregister_chrdev_region;
    }

    tun_cdev->ops = tun_fops;
    tun_cdev->owner = THIS_MODULE;

    //cdev_add返回的int值等于0成功，小于0为错误编码
    if(cdev_add(tun_cdev, tun_dev, MINOR_COUNT)){
        print_error("failed to add cdev\n");
        goto out_cdev_del;
    }

    tun_class = class_create(THIS_MODULE, "tun");
    //class_create返回有效指针为成功，返回错误指针失败。
    if(IS_ERR(tun_class)) {
        print_error("failed to create class\n");
        goto out_cdev_del;
    }

    struct device *tun_device;
    tun_device = device_create(tun_class, NULL, tun_dev, NULL, "tun%d", 0);
    //device_create返回有效指针为成功，返回错误指针失败。
    if(IS_ERR(tun_class)) {
        print_error("failed to create device\n");
        goto out_class_destroy;
    }
	return 0;
//如果成功调用cdev_alloc、cdev_init或cdev_add，则后续释放时需要使用cdev_del。cdev_init返回void，调用cdev_init一定成功。
out_class_destroy:
	class_destroy(tun_class);
out_cdev_del:
	cdev_del(tun_cdev);
out_unregister_chrdev_region:
	unregister_chrdev_region(tun_dev, MINOR_COUNT);
out_return:
	return -1;
}



static int char_device_exit(void)
{
    device_destroy(tun_class, tun_dev);
	class_destroy(tun_class);
	cdev_del(tun_cdev);
	unregister_chrdev_region(tun_dev, MINOR_COUNT);
	
	return 0;
}
