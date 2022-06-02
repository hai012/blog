#userdebug版本的ko模块不需要签名，以下宏打开后才需要ko模块签名
#CONFIG_MODULE_SIG = y
#CONFIG_MODULE_SIG_FORCE = y
#CONFIG_MODULE_SIG_ALL = y

OPS = -C /home/gonghj/project/mtk8195/kernel-4.19 O=/home/gonghj/project/mtk8195/out/target/product/xt501v2_64_bsp/obj/KERNEL_OBJ ARCH=arm64 CROSS_COMPILE=aarch64-linux-androidkernel- CLANG_TRIPLE=aarch64-linux-gnu- LD=ld.lld LD_LIBRARY_PATH=/home/gonghj/project/mtk8195/prebuilts/clang/host/linux-x86/clang-r383902/lib64: NM=llvm-nm OBJCOPY=llvm-objcopy CC=clang ROOTDIR=/home/gonghj/project/mtk8195 PATH=/home/gonghj/project/mtk8195/prebuilts/clang/host/linux-x86/clang-r383902/bin:/home/gonghj/project/mtk8195/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin:/usr/bin:/bin:/home/gonghj/project/mtk8195/prebuilts/build-tools/path/linux-x86:/home/gonghj/project/mtk8195/out/.path PROJECT_DTB_NAMES='mediatek/xt501v2_64_bsp'

all:
	make $(OPS) M=`pwd` modules

clean:
	make $(OPS) M=`pwd` modules clean
	rm -rf modules.order

# 参考内核源码drivers/char/ipmi/Makefile
# 要想把a.c, b.c编译成ab.ko, 可以这样指定:
# ab-y := a.o b.o
# obj-m += ab.o



obj-m += xxxxx.o

