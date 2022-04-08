

root@host1592400078:~# free -h
              total        used        free      shared  buff/cache   available
Mem:           478M         70M        283M        4.3M        124M        372M
Swap:          511M          0B        511M


不考虑Swap  total = used + free + buff/cache

used  =  所有进程所占物理内存  +  shared(共享库.so   进程间共享内存、队列、信号量)
free  真正空闲的物理内存
buff/cache  用于缓存磁盘块而占用的物理内存
available   站在应用程序角度还能用的内存。对应用程序来说buff/cache部分内存也是能够回收来利用的



swap：：：：：：：：：：：：：：：：：：：：：：

sysctl -q vm.swappiness查看参数的当前设置



永久修改vm.swappiness
echo "vm.swappiness = 0" >>   /etc/sysctl.conf
sysctl -p


swapoff -a   关掉swap

swapon -a 开启/etc/fstab上定义的所有swap

swapoff -a关闭所有swap

swapon -s 显示swap