#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>

#define print_error(fmt, args...) printk(KERN_ERR  "[key_drive]: %s() line: %d " fmt, __FUNCTION__, __LINE__, ##args)
#define print_info(fmt, args...)  printk(KERN_INFO "[key_drive]: %s() line: %d " fmt, __FUNCTION__, __LINE__, ##args)


#define MAJOR_BASE 0
#define MINOR_COUNT 1


static struct gpio_desc *key_gpio;


static struct dev_t key_dev;
static struct cdev *key_cdev;
static struct class * key_class;


static ssize_t key_read (struct file * filep, char __user * buf, size_t, loff_t * offset);
static static int key_poll (struct inode *node, struct file *file);
static int key_probe(struct platform_device *pdev);
static int key_remove(struct platform_device *pdev);
static int __init key_init(void);
static void __exit key_exit(void);


static const struct file_operations key_fops[] = {
	 {
		.owner      = THIS_MODULE,
		.open		= key_open,
		.read		= 
		.release	= key_release,
	},
};
static const struct of_device_id device_match_table[] = {
	{ .compatible = "zte,gpio_key" },
	{},
};
static struct platform_driver key_platform_driver = {
	.probe      = key_probe,
	.remove     = key_remove,
	.driver     = {
		.name = "zte,gpio_key",
        .of_match_table = device_match_table,
	},
};


unsigned int irq_handler_fn(void *)
{
	
	
}

unsigned int irq_handler_thread_fn(void *)
{
	
	
}





static ssize_t key_read (struct file * filep, char __user * buf, size_t, loff_t * offset)
{
	char buffer[4];
    
    copy_to_user(buf, bufer,);
    
}

static static int key_poll (struct inode *node, struct file *file)
{
	return 0;
}



/*
指针分三类，有效指针（指向内核空间其他page），NULL，错误指针(指向内核空间最后一个page)
*/

static int key_probe(struct platform_device *pdev)
{
	

	unsigned int irq_num;

	
#ifdef GET_IRQ_BY_GPIO //get virtual irq by gpio_to_irq
	keytest_gpio = gpiod_get(&pdev->dev, "keytest", 0);//  alloc/get/occupy gpio pin
	if (NULL == keytest_gpio) {
		goto out_return;
	}
	irq_num = gpio_to_irq(keytest_gpio);
#endif

#ifdef GET_IRQ_BY_RESOURCE //get virtual irq by platform_get_resource
	/*int platform_get_irq(struct platform_device *dev, unsigned int num)
	struct resource *platform_get_resource(struct platform_device *dev,
				       unsigned int type, unsigned int num);	*/
	struct resource * res = platform_get_resource(pdev,IORESOURCE_IRQ,0);
	irq_num = (unsigned int)(res->start);
#endif

#ifdef GET_IRQ_BY_DTB_NODE //get virtual irq by irq_of_parse_and_map
	struct device_node *nodePtr = pdev->node.of_node;//nodePtr = of_find_matching_node(NULL, device_match_table);也可以
	irq_num = irq_of_parse_and_map(nodePtr, 0);//irq_num = of_irq_get(nodePtr, 0);也可以,作用与irq_of_parse_and_map一样，只不过会在解析设备树后、映射map之前检查一下中断控制器的domain
#endif


	//register irq func , 2ways, request_threaded_irq  or request_irq
	int request_threaded_irq(unsigned int irq,        irq_handler_fn,
						 irq_handler_thread_fn, unsigned long irqflags,
						 const char *devname,     void *dev_id);

    //alloc_chrdev_region返回的int值等于0成功，小于0为错误编码
    if(alloc_chrdev_region(&key_dev, MAJOR_BASE, MINOR_COUNT, "key")){
        print_error("failed to alloc_chrdev_region\n");
        goto ;
    }

    //cdev_alloc返回有效指针成功，返回NULL失败
    key_cdev = cdev_alloc();
    if(NULL == key_cdev){
        print_error("failed to alloc cdev\n");
        goto out_unregister_chrdev_region;
    }

    key_cdev->ops = key_fops;
    key_cdev->owner = THIS_MODULE;

    //cdev_add返回的int值等于0成功，小于0为错误编码
    if(cdev_add(key_cdev, key_dev, MINOR_COUNT)){
        print_error("failed to add cdev\n");
        goto out_cdev_del;
    }

    
    key_class = class_create(THIS_MODULE, "key");
    //class_create返回有效指针为成功，返回错误指针失败。
    if(IS_ERR(key_class)) {
        print_error("failed to create class\n");
        goto out_cdev_del;
    }

    struct device *key_device;
    key_device = device_create(key_class, NULL, key_dev, NULL, "key%d", 0);
    //device_create返回有效指针为成功，返回错误指针失败。
    if(IS_ERR(key_class)) {
        print_error("failed to create device\n");
        goto out_class_destroy;
    }
	return 0;
//如果成功调用cdev_alloc、cdev_init或cdev_add，则后续释放时需要使用cdev_del。cdev_init返回void，调用cdev_init一定成功。
out_class_destroy:
	class_destroy(key_class);
out_cdev_del:
	cdev_del(key_cdev);
out_unregister_chrdev_region:
	unregister_chrdev_region(key_dev, MINOR_COUNT);
ERROR_alloc_chrdev_region:
	gpiod_put(key_gpio);
out_return:
	return -1;
}

static int key_remove(struct platform_device *pdev)
{
    device_destroy(key_class, key_dev);
	class_destroy(key_class);
	cdev_del(key_cdev);
	unregister_chrdev_region(key_dev, MINOR_COUNT);

	gpiod_put(key_gpio);
	
	return 0;
}


static int __init key_init(void)
{
    return platform_driver_register(&key_platform_driver);
}

static void __exit key_exit(void)
{
    platform_driver_unregister(&key_platform_driver);
}

module_init(key_init);
module_exit(key_exit);

MODULE_LICENSE("GPL");
