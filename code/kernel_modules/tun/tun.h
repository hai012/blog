


#define MAJOR_BASE 0
#define MINOR_COUNT 1


#define MACADDR0 0x08
#define MACADDR1 0x89
#define MACADDR2 0x89
#define MACADDR3 0x89
#define MACADDR4 0x89
#define MACADDR5 0x89



struct tun_priv {
    struct net_device   *net_dev;
    struct sk_buff      *send_skb_buff;
    struct dev_t        tun_dev;
    struct cdev         *tun_cdev;
    struct class        *tun_class;

}


#define print_error(fmt, args...) printk(KERN_ERR  "[tun_drive]: %s() line: %d " fmt, __FUNCTION__, __LINE__, ##args)
#define print_info(fmt, args...)  printk(KERN_INFO "[tun_drive]: %s() line: %d " fmt, __FUNCTION__, __LINE__, ##args)
