# qemu源码安装



```
//下载qemu
git clone https://git.qemu.org/git/qemu.git
cd qemu
git submodule init
git submodule update --recursive



安装依赖
apt install re2c
apt install ninja-build
apt install build-essential zlib1g-dev pkg-config libglib2.0-dev
aptinstall binutils-dev libboost-all-dev autoconf libtool libssl-dev libpixman-1-dev libpython-dev python-pip python-capstone virtualenv
apt install libpixman-1-dev
apt install bison flex
apt install meson
apt install libpixman-1-dev
apt install libpcap-dev libnids-dev libnet1-dev
apt install libattr1-dev
apt install libcap-ng-dev
apt install libslirp-dev


mkdir build

../configure --help

../configure  --target-list="arm-softmmu arm-linux-user" --enable-debug --enable-sdl --enable-tools --disable-curl  --enable-slirp

sudo make install
```









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

 apt install libssl-dev

apt install bc







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
iputils-tracepath isc-dhcp-client openssh-server curl vim dialog debconf
2.安装systemd init
root@ubunntu:/# apt install systemd
root@ubunntu:/# ln -s /lib/systemd/systemd /sbin/init
3.关闭systemd的networkd服务和resolvd
root@ubunntu:/# systemctl disable networkd-dispatcher
root@ubunntu:/# systemctl disable systemd-resolved.service
注意安装systemd后systemd-resolved.service会把/etc/resolv.conf 变成链接脚本指向其他文件，此时DNS解析会失败，这里关掉systemd-resolved后需要进行第0步配置DNS才能访问网络。
4.配置登录tty
root@ubunntu:/# ln -s /lib/systemd/system/getty@.service  /etc/systemd/system/getty.target.wants/getty@ttyS0.service


systemctl get-default
systemctl set-default multi-user.target
systemctl list-dependencies multi-user.target


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

C:\qemu\qemu-system-x86_64  -accel help

qemu-system-x86_64 -device help



性能顺序 kvm, xen, *hax*, hvf, nvmm, *whpx* ,tcg

选项参考

https://www.bilibili.com/read/cv12752511



# 模拟  arm vexpress开发板

https://www.qemu.org/docs/master/system/arm/vexpress.html



https://mirrors.edge.kernel.org/pub/linux/kernel/v5.x/linux-5.15.73.tar.xz

http://mirrors.ustc.edu.cn/ubuntu-cdimage/ubuntu-base/releases/focal/release/ubuntu-base-20.04.5-base-armhf.tar.gz



arm官方交叉编译工具链：

wget   https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-a/10.3-2021.07/binrel/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf.tar.xz

wget  https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-a/10.3-2021.07/binrel/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu.tar.xz



## 编译内核

```
ghj@ubunntu:~/work/qemu/linux-5.15.73$ export PATH=$PATH:/home/ghj/work/qemu/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin
ghj@ubunntu:~/work/qemu/linux-5.15.73$ export KBUILD_OUTPUT=../arm_build ARCH=arm CROSS_COMPILE=arm-none-linux-gnueabihf-
ghj@ubunntu:~/work/qemu/linux-5.15.73$ make -j8 vexpress_defconfig
ghj@ubunntu:~/work/qemu/linux-5.15.73$ make -j8 menuconfig  #打开

ghj@ubunntu:~/work/qemu/linux-5.15.73$ make -j8 zImage
ghj@ubunntu:~/work/qemu/linux-5.15.73$ make vexpress-v2p-ca9.dtb
ghj@ubunntu:~/work/qemu/linux-5.15.73$ make vexpress-v2p-ca15-tc1.dtb

ghj@ubunntu:~/work/qemu/linux-5.15.73$ make modules
ghj@ubunntu:~/work/qemu/linux-5.15.73$ make modules_install INSTALL_MOD_PATH=XXXXX


注意，高版本内核例如6.1.15需要打开CONFIG_UEVENT_HELPER选项才能使用busybox的mdev

  │ Symbol: UEVENT_HELPER [=n]                                                                                     │
  │ Type  : bool                                                                                                   │
  │ Defined at drivers/base/Kconfig:7                                                                              │
  │   Prompt: Support for uevent helper                                                                            │
  │   Location:                                                                                                    │
  │     -> Device Drivers                                                                                          │
  │       -> Generic Driver Options                                                                                │
  │ (1)     -> Support for uevent helper (UEVENT_HELPER [=n]) 
  
  
  
    │ Symbol: EARLY_PRINTK [=n]                                                                                      │
  │ Type  : bool                                                                                                   │
  │ Defined at arch/arm/Kconfig.debug:1928                                                                         │
  │   Prompt: Early printk                                                                                         │
  │   Depends on: DEBUG_LL [=n]                                                                                    │
  │   Location:                                                                                                    │
  │     -> Kernel hacking                                                                                          │
  │ (1)   -> arm Debugging                                                                                         │
  │         -> Early printk (EARLY_PRINTK [=n]) 
```



## 构建根文件系统

* ubuntu

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



* busybox

  ```
  wget https://busybox.net/downloads/busybox-1.36.0.tar.bz2
  tar  jxf busybox-1.36.0.tar.bz2
  cd busybox-1.36.0
  export ARCH=arm
  export CROSS_COMPILE=/home/haijie.gong/tools/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-
  make -j16 distclean
  make -j16 defconfig
  make -j16 menuconfig
  make -j16 
  make -j16 install
  cd ..
  
  mkdir rootfs
  dd if=/dev/zero of=rootfs.ext4 bs=1G count=1
  mkfs.ext4 rootfs.ext4
  
  su root
  mount rootfs.ext4 rootfs
  cp -rd busybox-1.36.0/_install/* rootfs/
  cd rootfs
  mkdir  dev  etc  home  lib   mnt  proc  root   sys  tmp   var
  ls ../busybox-1.36.0/examples/
  cp ../busybox-1.36.0/examples/*  etc
  mkdir usr/share
  mv etc/udhcp usr/share/udhcpc
  
  cat etc/inittab
  mkdir etc/init.d
  
  echo '
  mount -t proc   proc   /proc
  mount -t sysfs  sysfs  /sys
  mount -t tmpfs  tmpfs  /tmp
  mount -t tmpfs  tmpfs  /dev
  mkdir /dev/pts
  mount -t devpts devpts /dev/pts
  echo /sbin/mdev>/proc/sys/kernel/hotplug
  mdev -s
  ifconfig eth0 up
  udhcpc -i eth0 -s /usr/share/udhcpc/simple.script
  ' > etc/init.d/rcS
  
  echo '
  #!/bin/sh
  export HOSTNAME=farsight
  export USER=root
  export HOME=root
  export PS1="[$USER@$HOSTNAME \W]\# "
  #export PS1="[\[\033[01;32m\]$USER@\[\033[00m\]\[\033[01;34m\]$HOSTNAME\[\033[00m\ \W]\$ "
  PATH=/bin:/sbin:/usr/bin:/usr/sbin
  LD_LIBRARY_PATH=/lib:/usr/lib:$LD_LIBRARY_PATH
  export PATH LD_LIBRARY_PATH
  ' > etc/profile
  
  cd .. && umount rootfs
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





```
qemu-system-arm -M vexpress-a9  -accel tcg,thread=multi -cpu cortex-a9  -smp 4  -m 1G -kernel /home/haijie.gong/disk/mynet/linux-6.1.15/arch/arm/boot/zImage -dtb /home/haijie.gong/disk/mynet/linux-6.1.15/arch/arm/boot/dts/vexpress-v2p-ca9.dtb -nographic -serial mon:stdio   -append "console=ttyAMA0 rootfstype=ext4 root=/dev/mmcblk0  rw rootwait init=/sbin/init  loglevel=8"  -drive  file=/home/haijie.gong/disk/mynet/busybox/rootfs.ext4,format=raw,id=mysdcard -device sd-card,drive=mysdcard  -nic user
```



```
qemu-system-arm -M vexpress-a15  -accel tcg,thread=multi -cpu cortex-a15 -smp 2  -m 1G -kernel /home/haijie.gong/disk/mynet/linux-6.1.15/arch/arm/boot/zImage -dtb /home/haijie.gong/disk/mynet/linux-6.1.15/arch/arm/boot/dts/vexpress-v2p-ca15-tc1.dtb -nographic -serial mon:stdio   -append "console=ttyAMA0 rootfstype=ext4 root=/dev/mmcblk0  rw rootwait init=/sbin/init  loglevel=8"  -drive  file=/home/haijie.gong/disk/mynet/busybox/rootfs.ext4,format=raw,id=mysdcard -device sd-card,drive=mysdcard  -nic user
```







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



# qemu源码分析



http://wiki.100ask.org/Qemu











```
#define TYPE_MYNET "mynet"
#define mynet(obj) OBJECT_CHECK(mynet_state, (obj), "mynet")

typedef struct {
    SysBusDevice parent_obj;
    MemoryRegion mmio;
    qemu_irq irq;
    xxxxx
} mynet_state

static void mynet_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    //printf("mynet_class_init\n");
    dc->realize = mynet_realize;
}

static const TypeInfo mynet_info = {
    .name          = TYPE_MYNET,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_init = xxx_func,
    .instance_size = sizeof(mynet_state),
    .class_init    = mynet_class_init,
};


static void mynet_register_types(void)
{
    type_register_static(&mynet_info);
}
type_init(mynet_register_types)


void mynet_init(hwaddr base, qemu_irq irq)
{
    DeviceState *dev;
    SysBusDevice *s;

    dev = qdev_create(NULL, TYPE_MYNET);//instance_init
    qdev_init_nofail(dev);//realize

    s = SYS_BUS_DEVICE(dev);
    sysbus_mmio_map(s, 0, base);
    sysbus_connect_irq(s, 0, irq);
}
```



```
type_init

把TypeInfo变成TypeImpl放入type_table链表中
```









```
qdev_create
	qdev_try_create
		object_new
			object_new_with_type
				    type_initialize(type);//TypeImpl->class为空则创建class并回调class_init
    				obj = g_malloc(type->instance_size);//分配instance结构体，即mynet_state
    				object_initialize_with_type(obj, type->instance_size, type);
    					type_initialize//TypeImpl->class不为空则直接返回
    					object_init_with_type//回调instance_init


                	
                	



qdev_init_nofail
	object_property_set_bool(OBJECT(dev), true, "realized", &err);
		......
			回调class_init中设置的realize函数mynet_realize;
```

高版本已经废弃qdev_create/qdev_init_nofail







```
sysbus_init_child_obj
	object_initialize_child
		object_initialize_childv(parentobj, propname, childobj, size, type, errp,vargs);
			object_initialize(childobj, size, type);
				TypeImpl *type = type_get_by_name(typename);
				object_initialize_with_type(data, size, type);
    					type_initialize//为空则创建class并回调class_init,否则直接返回
    					object_init_with_type//回调instance_init






object_property_set_bool
```





