nfs端口固定化以便使用NAT转发访问内网nfs服务器


安装nfs的服务器上涉及到8个端口：


sunrpc/portmapper
本身就是固定端口111，无需处理

nfs
本身就是固定端口2049，无需处理


nfs-cb
# /etc/modprobe.d/local.conf
options nfs callback_tcpport=30000


statd
 # /etc/default/nfs-common
STATDOPTS="--port 30001 --outgoing-port 30002"


mountd
 # /etc/default/nfs-kernel-server
 RPCMOUNTDOPTS="--manage-gids  --port 30003"

lockd/nlockmgr
 # /etc/modprobe.d/local.conf
 options lockd nlm_udpport=30004   nlm_tcpport=30004


quotad
 # /etc/default/quota
 RPCRQUOTADOPTS="-p 30005"
 
 
 
 
 
 
 1、使用了非法端口，也就是使用了大于1024的端口。
这个错误，可以通过查看日志确认：
[root@local~ /]# cat /var/log/messages | grep mount
Jan 2 12:49:04 localhost mountd[1644]: refused mount request from 192.168.0.100 for /home/nfsshare/ (/home/nfsshare): illegal port 1689
 
解决办法：
修改配置文件/etc/exports，加入 insecure 选项,重启nfs服务，再尝试挂载。
/home/ghj     *(insecure,rw,async,no_hide,no_subtree_check,rw,all_squash,anonuid=1000,anongid=1000)


这里需要注意的是，将文件系统挂载到嵌入式linux目标版时，由于这里使用的ubuntu版本是19的，
ubuntu 17.10之后的nfs默认就只支持协议3和协议4，但是uboot默认启动的是协议2，要想让nfs支持协议2，
就得在/etc/default/nfs-kernel-server添加RPCNFSDOPTS="--nfs-version 2,3,4 --debug --syslog"，然后重新启动nfs服务。
具体步骤如下：
RPCNFSDOPTS="--nfs-version 2,3,4 --debug --syslog"




