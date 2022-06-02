



#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include "tun.h"





static int tun_init(void)
{


    if(vnet_init()) {

        return -1;
    }
    if(char_device_init()) {

        goto out_char_device_exit;
    }
    return 0;

out_vnet_exit:
    vnet_exit();
    return -1;
}




static void tun_exit(void)
{
    char_device_exit();
    vnet_exit();
}


module_init(tun_init);
module_exit(tun_exit);


MODULE_AUTHOR("ghj@163.com")
MODULE_LICENSE("GPL")

