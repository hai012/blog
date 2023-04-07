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

#define print_error(fmt, args...) printk(KERN_ERR "[led_drive]: %s() line: %d " fmt, __FUNCTION__, __LINE__, ##args)


static struct gpio_desc *led_gpio;
static struct proc_dir_entry   *ledlight_dir, *led_red_dir, *led_green_dir;


//一般应用层echo进来的字符串有一个'\0'，但为了保证安全在strcmp前需要判断是否以'\0'结尾
//为了确保一定以'\0'结尾需要copy_from_user后自己补上'\0'

//et3和xt501v2的电源指示灯都是红灯蓝灯，但老接口在应用层用/proc/ledlight/powergreen/state来操作蓝灯


static ssize_t proc_w_power_red (struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
	char buffer[4];

	if( len == 3 ) {
		copy_from_user(buffer, buf, 2);
		buffer[2]='\0';
		if( strcmp("on",buffer) ) {
			print_error("write ops error:%s",buffer);
			return -1;
		}
		gpiod_set_value(led_gpio, 0);//低电平有效，写逻辑0输出高电平,此时红灯亮
		return 3;
	}else if( len == 4 ) {
		copy_from_user(buffer, buf, 3);
		buffer[3]='\0';
		if( strcmp("off",buffer) ) {
			print_error("write ops error:%s",buffer);
			return -1;
		}
		gpiod_set_value(led_gpio, 1);//低电平有效，写逻辑1输出低电平，此时变成蓝灯
		return 4;
	}else {
		print_error("write ops length error:%d",len);
		return -1;
	}
}
static ssize_t proc_w_power_green (struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
	char buffer[4];

	if(len == 3) {
		copy_from_user(buffer, buf, 2);
		buffer[2]='\0';
		if( strcmp("on",buffer) ) {
			print_error("write ops error:%s",buffer);
			return -1;
		}
		gpiod_set_value(led_gpio, 1);//低电平有效，写逻辑1输出低电平,此时蓝灯亮
		return 3;
	}else if( len == 4 ) {
		copy_from_user(buffer, buf, 3);
		buffer[3]='\0';
		if( strcmp("off",buffer) ) {
			print_error("write ops error:%s",buffer);
			return -1;
		}
		gpiod_set_value(led_gpio, 0);//低电平有效，写逻辑0输出高电平，此时变成红灯
		return 4;
	}else {
		print_error("write ops length error:%d",len);
		return -1;
	}
}
static int led_open (struct inode *node, struct file *file)
{
	return 0;
}


static int led_release (struct inode *node, struct file *file)
{
	return 0;
}

static const struct file_operations led_proc_fops[] = {
	 {
		.owner          = THIS_MODULE,
		.open		= led_open,
		.write		= proc_w_power_red,
		.release	= led_release,
	},
	{
		.owner          = THIS_MODULE,
		.open		= led_open,
		.write		= proc_w_power_green,
		.release	= led_release,
	},
};


static int led_probe(struct platform_device *pdev)
{
	/* 设备树中定义有: led_gpio_group-gpios=<...>;	*/
	led_gpio = gpiod_get(&pdev->dev, "led_gpio_group", 0);
	if (IS_ERR_OR_NULL(led_gpio)) {
		//dev_err(&pdev->dev, "Failed to get GPIO for led\n");
		print_error("Failed to get GPIO for led\n");
		goto error1;
	}
	if( gpiod_direction_output(led_gpio, 0) ) {
		print_error("Failed to set GPIO direction\n");
		goto error2;
	}

	ledlight_dir = proc_mkdir("ledlight", NULL);
	if(IS_ERR_OR_NULL(ledlight_dir)){
		print_error("create ledlight failed\n");
		goto error2;
	}
	led_red_dir = proc_mkdir("powerred", ledlight_dir);
	if(IS_ERR_OR_NULL(led_red_dir)){
		print_error("create powerred failed\n");
		goto error3;
	}
	led_green_dir = proc_mkdir("powergreen",ledlight_dir);
	if(IS_ERR_OR_NULL(led_green_dir)){
		print_error("create powergreen failed\n");
		goto error4;
	}
	if(IS_ERR_OR_NULL( proc_create("state", 0777, led_red_dir, &led_proc_fops[0]) )  ){
		print_error("create red_state failed\n");
		goto error5;
	}
	if(IS_ERR_OR_NULL( proc_create("state", 0777, led_green_dir, &led_proc_fops[1]) )  ){
		print_error("create green_state failed\n");
		goto error6;
	}
	return 0;
error6:
	remove_proc_entry("state",led_red_dir);
error5:
	remove_proc_entry("powergreen",ledlight_dir);
error4:
	remove_proc_entry("powerred",ledlight_dir);
error3:
	remove_proc_entry("ledlight", NULL);
error2:
	gpiod_put(led_gpio);
error1:
	return -1;
}

static int led_remove(struct platform_device *pdev)
{
	remove_proc_entry("state",led_green_dir);
	remove_proc_entry("state",led_red_dir);
	remove_proc_entry("powergreen",ledlight_dir);
	remove_proc_entry("powerred",ledlight_dir);
	remove_proc_entry("ledlight", NULL);
	gpiod_put(led_gpio);
	return 0;
}

static const struct of_device_id device_matchs[] = {
	{ .compatible = "zte,gpio_led", },
	{},
};

static struct platform_driver led_platform_driver = {
	.probe      = led_probe,
	.remove     = led_remove,
	.driver     = {
		.name = "zte,gpio_led",
        	.of_match_table = device_matchs,
	},
};

static int __init led_init(void)
{
    return platform_driver_register(&led_platform_driver);
}

static void __exit led_exit(void)
{
    platform_driver_unregister(&led_platform_driver);
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");


