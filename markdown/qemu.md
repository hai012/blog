



# linux x86_64 host环境

sudo apt install qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils virtinst virt-manager
apt install qemu-user-static
apt install qemu-system-x86_64
apt install qemu-system-arm
apt install apt-file
apt install apt-file
apt-file update
apt-file search  \`which CMD\`



apt install build-essential
apt install libncurses5-dev
apt install bison
apt install flex
apt-get install libelf-dev
apt-get install libgmp-dev
apt install libmpc-dev





# 模拟 x86_64

版本

https://mirrors.edge.kernel.org/pub/linux/kernel/v5.x/linux-5.15.73.tar.xz

http://mirrors.ustc.edu.cn/ubuntu-cdimage/ubuntu-base/releases/focal/release/ubuntu-base-20.04.5-base-amd64.tar.gz





## 编译内核

```
ghj@ubunntu:~/work/qemu/linux-5.15.73$ export KBUILD_OUTPUT=../x86_64_build
ghj@ubunntu:~/work/qemu/linux-5.15.73$ make -j8  x86_64_defconfig
ghj@ubunntu:~/work/qemu/linux-5.15.73$ make -j8  bzImage
```



## 构建根文件系统

```
ghj@ubunntu:~/work/qemu$          mkdir mnt
ghj@ubunntu:~/work/qemu$          qemu-img create -f qcow2 qemu_disk.qcow2 100G
ghj@ubunntu:~/work/qemu$          sudo -s
root@ubunntu:/home/ghj/work/qemu# modprobe nbd
root@ubunntu:/home/ghj/work/qemu# qemu-nbd --connect=/dev/nbd0 qemu_disk.qcow2
root@ubunntu:/home/ghj/work/qemu# mkfs.ext4 /dev/nbd0
root@ubunntu:/home/ghj/work/qemu# mount /dev/nbd0 mnt
root@ubunntu:/home/ghj/work/qemu# tar Jxf ubuntu-base-20.04.5-base-amd64.tar.gz -C mnt


接下来对根文件系统进行配置
root@ubunntu:/home/ghj/work/qemu# cp /etc/apt/sources.list  mnt/etc/apt/
root@ubunntu:/home/ghj/work/qemu# chroot mnt
0.配置DNS
root@ubunntu:/# rm -rf /etc/resolv.conf
root@ubunntu:/# echo "nameserver 8.8.8.8"  >  /etc/resolv.conf 
root@ubunntu:/# echo "nameserver 114.114.114.114"  >>  /etc/resolv.conf 
1.安装必要的软件包
root@ubunntu:/# apt update
root@ubunntu:/# apt upgrade
root@ubunntu:/# apt install language-pack-en-base apt-utils net-tools iputils-ping
iputils-tracepath isc-dhcp-client openssh-server curl vim
2.安装systemd init
root@ubunntu:/# apt install systemd
root@ubunntu:/# ln -s /lib/systemd/systemd /sbin/init
3.关闭systemd的networkd服务和resolvd
root@ubunntu:/# systemctl disable networkd-dispatcher
root@ubunntu:/# systemctl disable systemd-resolved.service
注意安装systemd后systemd-resolved.service会把/etc/resolv.conf 变成链接脚本指向其他文件，此时DNS解析会失败，这里关掉systemd-resolved后需要进行第0步配置DNS才能访问网络。
4.配置登录tty
root@ubunntu:/# ln -s /lib/systemd/system/getty@.service  /etc/systemd/system/getty.target.wants/getty@ttyS0.service
5.启动rc-local开机自动DHCP
root@ubunntu:/# echo "[Install]"                  >> /lib/systemd/system/rc-local.service
root@ubunntu:/# echo "WantedBy=multi-user.target" >> /lib/systemd/system/rc-local.service
root@ubunntu:/# echo 'dhclient eth0' > /etc/rc.local
root@ubunntu:/# systemctl enable rc-local


根文件系统配置好了后卸载
root@ubunntu:/# exit
root@ubunntu:/home/ghj/work/qemu# umount mnt
root@ubunntu:/home/ghj/work/qemu# qemu-nbd -d /dev/nbd0
root@ubunntu:/home/ghj/work/qemu# rmmod nbd
```



## qemu启动

```
在windows上使用qemu去启动(如果在linux平台还可以加入--enable-kvm选项)
C:\qemu\qemu-system-x86_64.exe 
-nographic
-smp 8
-m 8G
-kernel N:\work\qemu\x86_64_build\arch\x86\boot\bzImage
-drive file=N:\work\qemu\qemu_disk.qcow2,format=qcow2
-append “root=/dev/sda rw rootfstype=ext4 console=ttyS0 init=/sbin/init”
-netdev user,id=tag0,net=192.168.76.0/24,dhcpstart=192.168.76.9,hostfwd=tcp::4022-:22
-device e1000,netdev=tag0
```

qemu-system-x86_64 -M help

qemu-system-x86_64  -accel help

qemu-system-x86_64 -device help





# 模拟  arm vexpress开发板

https://mirrors.edge.kernel.org/pub/linux/kernel/v5.x/linux-5.15.73.tar.xz

http://mirrors.ustc.edu.cn/ubuntu-cdimage/ubuntu-base/releases/focal/release/ubuntu-base-20.04.5-base-armhf.tar.gz

apt install qemu-user-static



## 编译内核

```
ghj@ubunntu:~/work/qemu/linux-5.15.73$ export PATH=$PATH:/home/ghj/work/qemu/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin
ghj@ubunntu:~/work/qemu/linux-5.15.73$ export KBUILD_OUTPUT=../arm_build ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabihf-
ghj@ubunntu:~/work/qemu/linux-5.15.73$ make -j8 vexpress_defconfig
ghj@ubunntu:~/work/qemu/linux-5.15.73$ make -j8 menuconfig  #打开

ghj@ubunntu:~/work/qemu/linux-5.15.73$ make -j8 zImage
ghj@ubunntu:~/work/qemu/linux-5.15.73$ make dtbs
ghj@ubunntu:~/work/qemu/linux-5.15.73$ make modules
ghj@ubunntu:~/work/qemu/linux-5.15.73$ make modules_install INSTALL_MOD_PATH=XXXXX
```



## 构建根文件系统

```
ghj@ubunntu:~/work/qemu$          mkdir mnt
ghj@ubunntu:~/work/qemu$          sudo -s
root@ubunntu:/home/ghj/work/qemu# dd if=/dev/zero of=root.img bs=1G count=2
root@ubunntu:/home/ghj/work/qemu# mkfs.ext4 root.img
root@ubunntu:/home/ghj/work/qemu# mount root.img mnt
root@ubunntu:/home/ghj/work/qemu# tar Jxf ubuntu-base-20.04.5-base-armhf.tar.gz -C mnt

接下来对根文件系统进行配置
root@ubunntu:/home/ghj/work/qemu# cp /usr/bin/qemu-arm-static mnt/usr/bin/
root@ubunntu:/home/ghj/work/qemu# vim mnt/etc/apt/source.list
添加如下ubuntu  arm  源：

# See http://help.ubuntu.com/community/UpgradeNotes for how to upgrade to
# newer versions of the distribution.
deb http://mirrors.ustc.edu.cn/ubuntu-ports focal main restricted
# deb-src http://mirrors.ustc.edu.cn/ubuntu-ports focal main restricted

## Major bug fix updates produced after the final release of the
## distribution.
deb http://mirrors.ustc.edu.cn/ubuntu-ports focal-updates main restricted
# deb-src http://mirrors.ustc.edu.cn/ubuntu-ports focal-updates main restricted

## N.B. software from this repository is ENTIRELY UNSUPPORTED by the Ubuntu
## team. Also, please note that software in universe WILL NOT receive any
## review or updates from the Ubuntu security team.
deb http://mirrors.ustc.edu.cn/ubuntu-ports focal universe
# deb-src http://mirrors.ustc.edu.cn/ubuntu-ports focal universe
deb http://mirrors.ustc.edu.cn/ubuntu-ports focal-updates universe
# deb-src http://mirrors.ustc.edu.cn/ubuntu-ports focal-updates universe

## N.B. software from this repository is ENTIRELY UNSUPPORTED by the Ubuntu
## team, and may not be under a free licence. Please satisfy yourself as to
## your rights to use the software. Also, please note that software in
## multiverse WILL NOT receive any review or updates from the Ubuntu
## security team.
deb http://mirrors.ustc.edu.cn/ubuntu-ports focal multiverse
# deb-src http://mirrors.ustc.edu.cn/ubuntu-ports focal multiverse
deb http://mirrors.ustc.edu.cn/ubuntu-ports focal-updates multiverse
# deb-src http://mirrors.ustc.edu.cn/ubuntu-ports focal-updates multiverse

## N.B. software from this repository may not have been tested as
## extensively as that contained in the main release, although it includes
## newer versions of some applications which may provide useful features.
## Also, please note that software in backports WILL NOT receive any review
## or updates from the Ubuntu security team.
deb http://mirrors.ustc.edu.cn/ubuntu-ports focal-backports main restricted universe multiverse
# deb-src http://mirrors.ustc.edu.cn/ubuntu-ports focal-backports main restricted universe multiverse

## Uncomment the following two lines to add software from Canonical's
## 'partner' repository.
## This software is not part of Ubuntu, but is offered by Canonical and the
## respective vendors as a service to Ubuntu users.
# deb http://archive.canonical.com/ubuntu focal partner
# deb-src http://archive.canonical.com/ubuntu focal partner

deb http://mirrors.ustc.edu.cn/ubuntu-ports focal-security main restricted
# deb-src http://mirrors.ustc.edu.cn/ubuntu-ports focal-security main restricted
deb http://mirrors.ustc.edu.cn/ubuntu-ports focal-security universe
# deb-src http://mirrors.ustc.edu.cn/ubuntu-ports focal-security universe
deb http://mirrors.ustc.edu.cn/ubuntu-ports focal-security multiverse
# deb-src http://mirrors.ustc.edu.cn/ubuntu-ports focal-security multiverse

最后切换root目录：
root@ubunntu:/home/ghj/work/qemu# chroot mnt

接下来安装软件定制root.img的过程与x86_64的类似，注意配置登录tty时用的ttyAMA0
root@ubunntu:/# ln -s /lib/systemd/system/getty@.service  /etc/systemd/system/getty.target.wants/getty@ttyAMA0.service
卸载时直接umount mnt即可
```



## qemu启动



```
在windows上使用qemu去启动
C:\qemu\qemu-system-arm.exe 
-M vexpress-a9
-nographic
-smp 4
-m 1G
-kernel N:\work\qemu\arm_build\arch\arm\boot\zImage
-dtb    N:\work\qemu\arm_build\arch\arm\boot\dts\vexpress-v2p-ca9.dtb
-sd     N:\work\qemu\rootfs.img
-append “root=/dev/mmcblk0 rw rootfstype=ext4 console=ttyAMA0 init=/sbin/init”
-nic user,net=192.168.76.0/24,dhcpstart=192.168.76.10,hostfwd=tcp::4022-:22
```



C:\qemu\qemu-system-arm.exe  -M help

C:\qemu\qemu-system-arm.exe  -accel help

C:\qemu\qemu-system-arm.exe -device help

C:\qemu\qemu-system-arm.exe  -M vexpress-a9 help

C:\qemu\qemu-system-arm.exe -M vexpress-a9 -nic model=?

C:\qemu\qemu-system-arm.exe -M xilinx-zynq-a9 -nic model=?



https://wiki.qemu.org/Documentation/Networking



# 构建initrd/initramfs

## initrd

dd if=/dev/zero of=initrd.img bs=10M count=5

mkfs.ext2 initrd.img

mount initrd.img mnt

填充mnt

umount mnt

gzip -d initrd.img > initrd.img.gz



使用qemu-system-xx 的-initrd选项指定initrd.img.gz文件 ，-append选项里 “root=/dev/ram rw rootfstype=ext2 console=ttyAMA0 init=/sbin/init”

## initramfs

填充好当前文件夹

find . | cpio -o -H newc | gzip > ../initramfs.cpio.gz

使用qemu-system-xx 的-initrd选项指定initramfs.img.gz文件 ，-append选项里不要指定root=xxx,   使用rdinit指定init进程 “console=ttyAMA0 rdinit=/sbin/init”



也可以选择直接在编译内核时将填充好的当前文件夹作为initramfs打包到内核镜像中
