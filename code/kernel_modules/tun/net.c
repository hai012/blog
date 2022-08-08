#include <linux/error.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/in.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/ip.h>
#include <linux/poll.h>

#include <asm/system.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/irq.h>


static struct net_device * net_dev;

static void vnet_send_packet(struct  sk_buff *skb, struct net_device *dev)
{
    static init cnt = 0;
    printk("vnet_send_packet cnt = %d\n", ++cnt);
    netif_stop_queue(dev);//停止该网卡的队列

    //使用网卡硬件发送出去,把skb的数据写入网卡，物理网卡并不能在写入后立马将数据发送出去。
}

void vnet_free_wakeup(tun_data_type *tun_data)
{
    dev_kfree_skb(tun_data->skb, tun_data->send_skb_buff);//释放skb

    /*当物理网卡把数据全部发送出去后，产生一个发送缓冲区空中断，在该中断里面唤醒网卡队列的函数。
    由于虚拟网卡写入skb后可以直接发送成功，所以这里直接调用唤醒网卡队列的函数*/
    netif_wake_queue(dev);//数据全部发送出去后唤醒网卡队列

    //更新网卡统计信息,ifconfig可见
    dev->stats.tx_packets++;
    dev->stats.tx_bytes += skb->len;
}


void vnet_recv_packet(struct sk_buff *skb)
{
    //构造一个假的 sk_buff 上报,参考ldd3
    //skb->data    |---mac header---|---IP header---|type|----------|
    netif_rx(struct sk_buff *skb);

    net_dev->status.rx_packets++;
    net_dev->status.rx_bytes += skb->len;
}


int vnet_init(void)
{
    /*1.分配一个net_device结构体*/
    net_dev = alloc_netdev(sizeof(struct tun_priv),"vnet%d",NULL);
    if(NULL == net_dev)
        return -ENOMEM;
    /*2.设置*/
    ether_setup(net_dev);
    //2.1发包函数
    net_dev->hard_start_xmit =  vnet_send_packet;
    //mac地址
    net_dev->dev_addr[0] = MACADDR0;
    net_dev->dev_addr[1] = MACADDR1;
    net_dev->dev_addr[2] = MACADDR2;
    net_dev->dev_addr[3] = MACADDR3;
    net_dev->dev_addr[4] = MACADDR4;
    net_dev->dev_addr[5] = MACADDR5;
    //flag
    net_dev->flags        |= IFF_NOARP;
    net_dev->features     |= NETIF_F_NO_CSUM;
    net_dev->hard_header_cache = NULL;      /* Disable ARP caching */
    /*3.注册*/
    return register_netdev(net_dev);//获得一个锁后调用register_netdevice(net_dev);
}



void vnet_exit()
{
    unregister_netdev(net_dev);//获得一个锁后调用unregister_netdevice(net_dev);
    free_netdev();
}


