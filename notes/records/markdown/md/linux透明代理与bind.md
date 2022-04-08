相关命令

route

iptable

ip route show

四表五链

![iptables](D:\笔记\markdown\picture\iptables.PNG)

![route](D:\笔记\markdown\picture\route.PNG)



实现透明代理(应用不知情的情况下代理)：

1、修改路由表，让数据包目标地址为远端代理服务器IP的包发往真正的物理网络设备，让目标地址为其他的数据包发往lo环回设备。

2、接着本地运行本地代理客户端程序，打开套接字选项IP_TRANSPARENT，绑定监听INADDR_ANY。



