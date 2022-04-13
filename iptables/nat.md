```
https://blog.csdn.net/jk110333/article/details/8229828
https://blog.51cto.com/u_11060853/2114638

1.上外网 环境

nat表具有转发的功能，让服务器具有路由器的功能。



两台机器A和B。

A有两块网卡，网卡a:192.168.88.130   网卡b:192.168.100.2

B有一块网卡，网卡c:192.168.100.2

192.168.88.130是可以上外网的，网卡c只能连通网卡b，并不能上网。



1.1 目标：

让机器B通过机器A转发，达到可以上网的功能。



1.2  nat转发 上外网

1.给两台机器都添加1块网卡，自定义lan段





2.设置机器A新网卡的IP地址

# ifconfig ens37 192.168.100.2



3.停掉原来的网卡，设置机器B新网卡的地址。

# ifdown ens33

# ifconfig ens37 192.168.100.3



4.测试新网卡是否能通

A：

# ping 192.168.100.3

B:

# ping 192.168.100.2



5.打开A机器的内核转发

# echo "1" > /proc/sys/net/ipv4/ip_forward



6.增加机器A的转发规则，

# iptables -F

# iptables -t nat -A POSTROUTING -s 192.168.100.0/24 -o ens33 -j MASQUERADE



7.设置机器B的网关,利用机器A的网卡b出去，所以网关的ip就是网卡b的IP。

# route add default gw 192.168.100.2



8.设置机器B的DNS。

# vim /etc/resolv.conf

nameserver  119.29.29.29



9.测试

# ping www.qq.com

成功ping通外网。





2.端口映射

把A机器的1122端口映射成B机器的22端口，连接A机器的1122端口，实际连接的是B机器的22端口。

环境同上，C机器就是PC主机。



2.1 目标

C机器连接A机器上，实际连接的是B机器。



2.2 nat表 端口映射实例



1.给两台机器都添加1块网卡，自定义lan段

同上



2.设置机器A新网卡的IP地址

# ifconfig ens37 192.168.100.2



3.停掉原来的网卡，设置机器B新网卡的地址。

# ifdown ens33

# ifconfig ens37 192.168.100.3



4.测试新网卡是否能通

A：

# ping 192.168.100.3

B:

# ping 192.168.100.2



5.打开A机器的内核转发

# echo "1" > /proc/sys/net/ipv4/ip_forward



6.增加机器A的转发规则

# iptables -t nat -F

# iptables -t nat -A PREROUTING -d 192.168.88.130 -p tcp --dport 1122 -j DNAT --to 192.168.100.3:22         #把88.130的1122端口映射成100.3的22端口

# iptables -t nat -A POSTROUTING -s 192.168.100.3 -j SNAT --to 192.168.88.130 #从100.3出去的包到88.130上去。



7.设置机器B的网关,利用机器A的网卡b出去，所以网关的ip就是网卡b的IP。

# route add default gw 192.168.100.2



8.设置机器B的DNS。

# vim /etc/resolv.conf

nameserver  119.29.29.29



9.测试，通过ifconfig命令，查到是B机器的IP。



```

