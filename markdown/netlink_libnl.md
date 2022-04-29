参考官方介绍文档 http://www.infradead.org/~tgr/libnl/doc/core.html#core_netlink_fundamentals





### 1. netlink

#### 1.1 创建netlink套接字创建

       #include <sys/types.h>         
       #include <sys/socket.h>
    
       int socket(int domain, int type, int protocol);
其中，

domain可以是AF_NETLINK或PF_NETLINK

type可以是SOCK_RAW或SOCK_DGRAM。

protocol指定netlink协议类型。定义在include/uapi/linux/netlink.h，可以自己在该文件中增加自定义的protocol，目前 netlink 协议族支持32种协议类型,内核使用了21中，如下：

```


#define NETLINK_ROUTE       0   // Routing/device hook ，用于设置和查询路由表等网络核心模块
#define NETLINK_UNUSED      1   /* Unused number                */
#define NETLINK_USERSOCK    2   /* Reserved for user mode socket protocols  */
#define NETLINK_FIREWALL    3   /* Unused number, formerly ip_queue     */
#define NETLINK_SOCK_DIAG   4   /* socket monitoring                */
#define NETLINK_NFLOG       5   /* netfilter/iptables ULOG */
#define NETLINK_XFRM        6   /* ipsec */
#define NETLINK_SELINUX     7   /* SELinux event notifications */
#define NETLINK_ISCSI       8   /* Open-iSCSI */
#define NETLINK_AUDIT       9   /* auditing */
#define NETLINK_FIB_LOOKUP  10
#define NETLINK_CONNECTOR   11
#define NETLINK_NETFILTER   12  /* netfilter subsystem */
#define NETLINK_IP6_FW      13
#define NETLINK_DNRTMSG     14  /* DECnet routing messages */
#define NETLINK_KOBJECT_UEVENT  15  // Kernel messages to userspace，用于uevent消息通信
#define NETLINK_GENERIC     16
/* leave room for NETLINK_DM (DM Events) */
#define NETLINK_SCSITRANSPORT   18  /* SCSI Transports */
#define NETLINK_ECRYPTFS    19
#define NETLINK_RDMA        20
#define NETLINK_CRYPTO      21  /* Crypto layer */

#define NETLINK_INET_DIAG   NETLINK_SOCK_DIAG

#define MAX_LINKS 32
```



#### 1.2 将netlink socket和本地地址绑定

       #include <sys/types.h> 
       #include <sys/socket.h>
    
       int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

sockfd是前面创建socket返回的fd，后面两个参数用于描述套接字地址及其长度，netlink socket地址结构体如下：

```
struct sockaddr_nl {
     __kernel_sa_family_t    nl_family; /* 固定取AF_NETLINK或者PF_NETLINK,无差别 */
     unsigned short    nl_pad;          /* 填充字段，取zero        */
     __u32          nl_pid;          /* port ID */
     __u32          nl_groups;       /* multicast groups mask ，用于指定多播组，每一个bit对应一个多播组，如果设置为0，表示不加入任何多播组 */
};
```

字段 nl_pid 可以是接收或发送消息的进程的 ID，如果希望内核处理消息或多播消息，就把该字段设置为 0，否则设置为处理消息的进程 ID。传递给 bind 函数的地址的 nl_pid 字段应当设置为本进程的进程 ID，这相当于 netlink socket 的本地地址。但是，对于一个进程的多个线程使用 netlink socket 的情况，字段 nl_pid 则可以设置为其它的值，如： pthread_self() << 16 | getpid()或其他唯一的随机值，因此字段 nl_pid 实际上未必是进程 ID,它只是用于区分不同的接收者或发送者的一个标识，只是可一般用进程ID标识而已，实际上用户可以根据自己需要设置该字段。



字段 nl_groups 用于指定多播组，bind 函数用于把调用进程加入到该字段指定的多播组，如果设置为 0，表示调用者不加入任何多播组。



#### 1.3 发送netlink消息

       #include <sys/types.h>
       #include <sys/socket.h>
    
       ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
       
    struct msghdr {
        void         *msg_name;       /* optional address */
        socklen_t     msg_namelen;    /* size of address */
        struct iovec *msg_iov;        /* scatter/gather array */
        size_t        msg_iovlen;     /* # elements in msg_iov */
        void         *msg_control;    /* ancillary data, see below */
        size_t        msg_controllen; /* ancillary data buffer len */
        int           msg_flags;      /* flags (unused) */
    };
    
    struct iovec
    {
        void __user *iov_base;    /* BSD uses caddr_t (1003.1g requires void *) */
        __kernel_size_t iov_len; /* Must be size_t (1003.1g) */
    };

发送netlink消息需要填充struct msghdr，然后调用标准socket通信API sendmsg来发送。





* 填充struct msghdr的msg_name和msg_namelen，即需要发送的远程地址

  ```
  struct msghdr msg;
  struct sockaddr_nl peer_addr;
  bzero(&peer_addr, sizeof(peer_addr));//port ID为0说明与内核通信,group为0说明单播
  peer_addr.nl_family = AF_NETLINK;
  msg.msg_name = (void *)&peer_addr;
  msg.msg_namelen = sizeof(peer_addr);
  ```



* 填充struct msghdr的msg_iov，即需要发送的内容

  netlink  message 的格式为struct nlmsghdr  + pad+ payload + pad，struct nlmsghdr如下：

  ```
  struct nlmsghdr
  {
  __u32 nlmsg_len; /* Length of message 即 sizeof(struct nlmsghdr)+align+data+align */
  __u16 nlmsg_type; /* Message type*/
  __u16 nlmsg_flags; /* Additional flags */
  __u32 nlmsg_seq; /* Sequence number */
  __u32 nlmsg_pid; /* Sending process PID */
  };
  具体怎样填充参考 http://www.infradead.org/~tgr/libnl/doc/core.html#core_netlink_fundamentals
  ```

  

  ```
  char info[] = "An example message";
  int buffer_len = NLMSG_SPACE(strlen(info));//struct nlmsghdr + pad + payload(即strlen(info)） + pad
  struct nlmsghdr nlhdr;
  nlhdr = (struct nlmsghdr *)malloc(buffer_len);
  
  //填充payload
  strcpy(NLMSG_DATA(nlhdr),info);
  
  //填充struct nlmsghdr
  #define MSG_SETCFG      0x11
  nlhdr->nlmsg_len = buffer_len; 
  nlhdr->nlmsg_type = MSG_SETCFG;//使用自定义的message type
  nlhdr->nlmsg_flags = NLM_F_APPEND;
  nlhdr->nlmsg_pid = getpid(); /* self pid */
  
  
  struct iovec iov;
  iov.iov_base = (void *)nlhdr;//iov_base指向的buffer中必须有且仅有一个完整的netlink frame
  iov.iov_len = nlh->nlmsg_len;
  msg.msg_iov = &iov;//msg_iov可以指向一个struct iovec数组
  msg.msg_iovlen = 1;//表示msg.msg_iov指向的struct iovec数组只有一个struct iovec结构体
  ```

一个struct msghdr的msg_iov可以指向多个struct iovec，一个struct iovec的iov_base只能指向一个完整的struct nlmsghdr+pad+payload1+pad这种组合





* 最后开始发送

  ```
  sendmsg(fd, &msg, 0);
  ```

  



#### 1.4 接收netlink消息 

接收与发送类似，直接参考demo。





#### 1.5 辅助netlink编程的一些宏

struct iovec中的iov_base指向的buffer中的netlink message 由struct nlmsghdr   +  pad+  payload + pad组成，其中payload 按照规范应尽可能地使用Protocol Header和Attributes的这种模式(上文为了简单并未使用)。Protocol Header与创建netlink套接字时指定的protocol和内核子系统实现有关，不同的protocol/内核子系统使用不同的Protocol Header，如果是其他自定义的protocol那么需要自己去定义Protocol Header或者不使用Protocol Header和Attributes的这种模式。

![image-20220425180148989](netlink_libnl.assets/image-20220425180148989.png)

注意，最后的pad无论是放在payload后面还是payload中最后一个Attributes后面还是二者后面都放一个pad，最终整个netlink frame长度都相等。





常见的Protocol  header有这些种:

struct rtmsg
struct tcmsg
struct tcamsg
struct nfgenmsg
struct ifaddrmsg
struct ifinfomsg
struct ndmsg
struct ndtmsg
struct rtgenmsg
struct xfrm_usersa_info
struct xfrm_usersa_id
struct xfrm_userpoilcy_id





* 宏NLMSG_ALIGN(len)

  用于得到不小于len且4 byte对齐的最小数值。

```
#define NLMSG_ALIGNTO 4
#define NLMSG_ALIGN(len) ( ((len)+NLMSG_ALIGNTO-1) & ~(NLMSG_ALIGNTO-1) )
```



* 宏NLMSG_HDRLEN

  返回sizeof(struct nlmsghdr)+pad的长度

```
#define NLMSG_HDRLEN ((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))
```



* 宏NLMSG_LENGTH(len)

  用于计算payload长度为len时实际的消息总长度sizeof(struct nlmsghdr)+pad+payload。

```
#define NLMSG_LENGTH(len) ((len)+NLMSG_HDRLEN)
```



* 宏NLMSG_SPACE(len)

  返回payload长度为len时实际的消息总长度sizeof(struct nlmsghdr)+pad+payload+pad，struct nlmsghdr中的nlmsg_len也是指这个宏

```
#define NLMSG_SPACE(len) NLMSG_ALIGN(NLMSG_LENGTH(len))
```



* 宏NLMSG_DATA(nlh)

  用于通过某个struct nlmsghdr首地址nlh得到其payload首地址，即nlh+sizeof(struct nlmsghdr)+pad。

```
#define NLMSG_DATA(nlh) ((void*)(((char*)nlh) + NLMSG_LENGTH(0)))
```



* 宏NLMSG_NEXT(nlh,len)

  用于在buffer中从某一个struct nlmsghdr首地址得到下一个消息的struct nlmsghdr首地址，同时buffer剩余len也减少为剩余消息的总长度。

msg.msg_iov.iov_base 指向的buffer内可以有多个netlink frame,例如（header1+pad+payload1+pad)+ (header2+pad+payload2+pad) +...

NLMSG_NEXT(nlh,len)中的nlh指buffer中某个struct nlmsghdr的首地址，len是指buffer总长度除去nlh前面msg剩下的长度(即nlh所在frame和剩余frame的总长度)。执行NLMSG_NEXT(nlh,len)后整体返回的是一个指向nlh所在frame的下一个frame的struct nlmsghdr的首地址，且拥有副作用让len减少了。

```
#define NLMSG_NEXT(nlh,len) ((len) -= NLMSG_ALIGN((nlh)->nlmsg_len), /
(struct nlmsghdr*)(((char*)(nlh)) + NLMSG_ALIGN((nlh)->nlmsg_len)))
```





* 宏NLMSG_OK(nlh,len)

  nlh指buffer中某个struct nlmsghdr的首地址，len是指buffer总长度除去nlh及其前面msg剩下的长度，用于判断nlh后面是否还有netlink frame，如果以下三个条件同时满足则则返回1，否则返回0，该宏一般在接收netlink frame后在buffer中解析时使用：

  1.  len是否大于等于sizeof(struct nlmsghdr)

  2.  (nlh)->nlmsg_len否大于等于sizeof(struct nlmsghdr)

  3.  (nlh)->nlmsg_len是否小于等于 len

```
#define NLMSG_OK(nlh,len) ((len) >= (int)sizeof(struct nlmsghdr) && /
(nlh)->nlmsg_len >= sizeof(struct nlmsghdr) && /
(nlh)->nlmsg_len <= (len))
```





* 宏NLMSG_PAYLOAD(nlh,len)

  该宏用于返回payload的长度减去len后剩下的长度。一般在payload是Protocol Header和Attributes这种模式中使用。

```
#define NLMSG_PAYLOAD(nlh,len) ((nlh)->nlmsg_len - NLMSG_SPACE((len)))
```





#### 1.6 应用层demo

https://www.linuxjournal.com/article/8498?page=0,1



netlink套接字只能单播--单播通信和同一组播内互相通信，无论是收还是发。



* 初始化

```
int fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);//通用 netlink 协议

//填充本地套接字并绑定，本地套接字地址用于配置当前进程中套接字的属性，类似于设置当地快递点的地址及身份
struct sockaddr_nl local_addr;
bzero(&local_addr, sizeof(local_addr));
local_addr.nl_family = AF_NETLINK;
local_addr.nl_pad = 0;
local_addr.nl_pid = getpid();//指定当地快递点的身份Port ID
local_addr.nl_groups = 0;//指定当地快递点是单播还是组播，如果是组播需要指定加入哪个组播
int ret = bind(fd, (struct sockaddr*) &local_addr, sizeof(local_addr));
```

* 发送

```
//填充peer_addr，该地址类似于目的地的快递地址
struct sockaddr_nl peer_addr;
bzero(&peer_addr, sizeof(peer_addr));//peer_addr.nl_pid为0表示目的地址是内核，peer_addr.nl_groups表示单播
peer_addr.nl_family = AF_NETLINK;

//分配并填充buffer
char * info = "An example message";
int buffer_len = NLMSG_SPACE( strlen(buffer) ); //sizof(struct nlmsghdr) + align + data_len + align
struct nlmsghdr nlhdr = (struct nlmsghdr *)malloc(buffer_len);
strcpy(NLMSG_DATA(nlhdr),info); //不使用header,直接拷贝到data字段中
nlhdr->nlmsg_len = buffer_len;
nlhdr->nlmsg_pid = getpid();
nlhdr->nlmsg_flags = 0;


//填充struct iovec
struct iovec iov;
iov.iov_base = (void *)nlhdr;
iov.iov_len = nlhdr->nlmsg_len;


//填充struct msghdr
struct msghdr msg;
memset(&msg, 0, sizeof(msg));
msg.msg_name = (void *)&(peer_addr);
msg.msg_namelen = sizeof(peer_addr);
msg.msg_iov = &iov;
msg.msg_iovlen = 1;


//发送，使用当地快递点(fd)把信息(msg.iov)发送到目的快递点(msg.msg_name)
sendmsg(fd, &msg, 0);
```

发送的时候能够知道一次要发送多长才是一个完整的netlink frame，因此iov需要指向一个buffer，该buffer中有且只有一个完整的netlink frame。





* 接收

```
#define MAX_BUFFER_LEN 1024
struct sockaddr_nl peer_addr;
struct msghdr msg;
struct iovec iov;
struct nlmsghdr *nlhdr = (struct nlmsghdr *)malloc(MAX_BUFFER_LEN);
iov.iov_base = (void *)nlhdr;
iov.iov_len = MAX_BUFFER_LEN;
msg.msg_name = (void *)&(peer_addr);
msg.msg_namelen = sizeof(peer_addr);
msg.msg_iov = &iov;
/* 虽然只接收一个套接字msg，但接收后iov_base指向的buffer中可以有多条netlink frame,这取决于发送端是否把多个netlink frame放入一个套接字进行发送，多netlink frame的结构被定义成这样：nlmsghdr1+align+data_len1+align  + nlmsghdr2+align+data_len2+align + ... */
msg.msg_iovlen = 1;
recvmsg(fd, &msg, 0); //recvmsg时iov和peer_addr将被填充
```



,注意，由于事先不知道需要接收的长度，如果iov.iov_len比较短那么接收后iov.iov_base指向的buffer中可能并不是一个完整的netlink frame，需要再次接收，然后把多次接收的合在一起进行解析，解析时使用NLMSG_OK宏判断某个netlink frame后面是否还有netlink frame，如果payload是Protocol Header和Attributes这种模式，还能使用NLMSG_PAYLOAD宏辅助解析。





### 2. libnl

推荐netlink 套接字通信都使用libnl系列的库来进行而不是直接使用socket API操作netlink套接字，libnl把使用socket API操作netlink套接字的操作进行了一层封装。libnl使用方法参考官网文档详细介绍。

对于内核中常用的netlink protocol, 例如route,genl,nf，可以使用libnl-xxx，它们是对libnl更进一步的封装。

对于如果想直接操作netlink ，可以使用libnl。





![img](netlink_libnl.assets/layer_diagram.png)
