
Device tree sources in the kernel deviate from the regular syntax, by using the cpp preprocessor for includes and substitution. This proceeds as follows:

IDE=<your-device-name>
SRC=$IDE.dts
TMP=$IDE.tmp.dts
DST=$IDE.dtb

cpp -nostdinc -I include -undef -x assembler-with-cpp $SRC > $TMP
dtc -O dtb -b 0 -o $DST $TMP
rm $TMP

编译DTB和DTBO
dtc -@ -O dtb -o my_main_dt.dtb my_main_dt.dts
dtc -@ -O dtb -o my_overlay_dt.dtbo my_overlay_dt.dts
查看叠加后的DTS文件
dtsFile=my_main_dt.dts
dtsoFile=my_overlay_dt.dts
mergedFile=my_merged_dt.dts
cp   ${dtsFile}     my_overlay_dt_tmp.dts
sed  -i   '/\/dts-v1\/;/d'    my_overlay_dt_tmp.dts
sed  -i   '/\/plugin\/;/d'    my_overlay_dt_tmp.dts
cp    ${dtsFile}      my_main_dt_include.dts
echo "/include/ my_overlay_dt_tmp.dts" >> my_main_dt_include.dts
dtc -@ -O dtb -o  my_merged_dt.dtb        my_main_dt_include.dts
dtc -O dts  -o    ${mergedFile}           my_merged_dt.dtb
rm   my_overlay_dt_tmp.dts   my_main_dt_include.dts  my_merged_dt.dtb

echo 0 > /proc/sys/kernel/printk
4       4       1       7
 /proc/sys/kernel/printk 会显示4个数值（可由 echo 修改），分别表示
当前控制台日志级别
未明确指定日志级别的默认消息日志级别
最小（最高）允许设置的控制台日志级别
引导时默认的日志级别
当 printk() 中的消息日志级别小于当前控制台日志级别时，printk 的信息（要有/n符）就会在控制台上显示。
但无论当前控制台日志级别是何值，通过 /proc/kmsg （或使用dmesg）总能查看。另外如果配置好并运行
了syslogd 或 klogd，没有在控制台上显示的 printk 的信息也会追加到 /var/log/messages.log 中。

进程
pid, ppid, pgid, sid,     uid, gid, sgid

用户
uid gid

文件
拥有者uid   拥有者同组gid  suid  sgid

userid：就是有多少个实际的用户，即手机里的主机、访客等多用户。
uid：等于linux进程的uid
appid：跟app相关，包名相同的appid都一样，即使是不同用户。



#include <generated/autoconf.h>
/dts-v1/;

/plugin/;
#include <dt-bindings/apusys/mt8195-apusys-power.h>
#include <dt-bindings/gpio/gpio.h>
#include <dt-bindings/interrupt-controller/irq.h>
#include <dt-bindings/interrupt-controller/arm-gic.h>
#include <dt-bindings/phy/phy.h>
#include <dt-bindings/pinctrl/mt8195-pinfunc.h>
#include <dt-bindings/phy/phy.h>
#include <dt-bindings/spmi/spmi.h>

#include "mediatek/mt6359.dtsi"---------#include <dt-bindings/iio/mt635x-auxadc.h>
#include #include <dt-bindings/clock/mt8195-clk.h>


pio  pinctrl  gpio

kernel-4.19/include/linux/pinctrl/pinconf-generic.h
kernel-4.19/include/dt-bindings/pinctrl/mt8195-pinfunc.h
kernel-4.19/arch/arm64/boot/dts/mediatek/xt501v2_64_bsp.dts

&pio {
...
	led_state_label: led_state {
		pins_cmd_dat {
			pinmux = <PINMUX_GPIO15__FUNC_GPIO15>;
			bias-disable;
			input-disable;
			input-schmitt-disable;
			drive-open-drain;
			output-enable;
			output-high;
		};
	};
...
}

&odm {
...
	led_device {
		compatible = "zte,gpio_led";
		pinctrl-names = "default";
		pinctrl-0 = <&led_state_label>;
		led_gpio_group-gpios = <&pio 15 GPIO_ACTIVE_LOW>;
	};

}


adb root && adb push Z:\project\test\leddrv.ko /data



Error: arch/arm64/boot/dts/mediatek/xt501v2_64_bsp.dts:238.1-2 syntax error






cat /sys/devices/platform/10005000.pinctrl/mt_gpio

echo mode 15 0 > /sys/devices/platform/10005000.pinctrl/mt_gpio
echo dir  15 1 > /sys/devices/platform/10005000.pinctrl/mt_gpio
echo pullen 15 0 > /sys/devices/platform/10005000.pinctrl/mt_gpio
echo out  15 0 > /sys/devices/platform/10005000.pinctrl/mt_gpio






strip_output := $(LOCAL_BUILT_MODULE)
 python vendor/mediatek/proprietary/scripts/kernel_tool/rm_ko_sig.py $@ && $(KOUT)/scripts/sign-file sha256 $(PRIVATE_SIGN_KEY) $(KOUT)/certs/signing_key.x509 $@;

 



device/mediatek/build/tasks/dtbo.mk
 kernel-4.19/Android.mk
 kernel-4.19/kenv.mk

 

 

 static const struct file_operations led_proc_fops[] = {
	 {
		.open		= led_proc_open,
		.write		= proc_w_power_red,
		.read		= NULL,
		.llseek		= seq_lseek,
		.release	= single_release,
	},
	{
		.open		= led_proc_open,
		.write		= proc_w_power_green,
		.read		= NULL,
		.llseek		= seq_lseek,
		.release	= single_release,
	},
};



struct proc_dir_entry *ledlight_dir, *led_red_dirs, *led_green_dirs, *redState, *greenState;

ledlight_dir   = proc_mkdir("ledlight", NULL);
led_red_dirs   = proc_mkdir("red", ledlight_dir);
led_green_dirs = proc_mkdir("green", ledlight_dir);

redState       = proc_create("state", 0777, led_red_dirs, &led_proc_fops[0]);
greenState     = proc_create("state", 0777, led_green_dirs, &led_proc_fops[1]);







  struct platform_driver {
	int (*probe)(struct platform_device *);
	int (*remove)(struct platform_device *);
	void (*shutdown)(struct platform_device *);
	int (*suspend)(struct platform_device *, pm_message_t state);
	int (*resume)(struct platform_device *);
	struct device_driver driver;
	const struct platform_device_id *id_table;
	bool prevent_deferred_probe;
 };

struct platform_device_id {
  	char name[PLATFORM_NAME_SIZE];
  	kernel_ulong_t driver_data;
};


struct device_driver {
 	const char		*name;
 	struct bus_type		*bus;

 	struct module		*owner;

 	const struct of_device_id	*of_match_table;

 	int (*probe) (struct device *dev);
 	void (*sync_state)(struct device *dev);
 	int (*remove) (struct device *dev);
 	void (*shutdown) (struct device *dev);
 	int (*suspend) (struct device *dev, pm_message_t state);
 	int (*resume) (struct device *dev);
.......
 };

  struct of_device_id {
  	char	name[32];
  	char	type[32];
  	char	compatible[128];
  	const void *data;
  };


  struct platform_device {
  	const char	*name;
  	int		id;
  	bool		id_auto;
  	struct device	dev;
  	u32		num_resources;
  	struct resource	*resource;

  	const struct platform_device_id	*id_entry;
  	char *driver_override; /* Driver name to force a match */

  	/* MFD cell pointer */
  	struct mfd_cell *mfd_cell;

  	/* arch specific additions */
  	struct pdev_archdata	archdata;
  };


 struct device {

  	struct device_node	*of_node; /* associated device tree node */

  };

  struct device_node {
  	const char *name;
  	const char *type;
  	phandle phandle;
  	const char *full_name;
  	struct fwnode_handle fwnode;

  	struct	property *properties;
  	struct	property *deadprops;	/* removed properties */
  	struct	device_node *parent;
  	struct	device_node *child;
  	struct	device_node *sibling;
  #if defined(CONFIG_OF_KOBJ)
  	struct	kobject kobj;
  #endif
  	unsigned long _flags;
  	void	*data;
  #if defined(CONFIG_SPARC)
  	const char *path_component_name;
  	unsigned int unique_id;
  	struct of_irq_controller *irq_trans;
  #endif
  };



总线上平台设备与平台驱动匹配过程:

struct platform_device中的char *driver_override
struct platform_driver中的struct device_driver driver中的const char  *name


struct platform_device中的struct device dev中的struct device_node  *of_node中的compatible属性(由设备树节点下的compatible属性解析生成)
struct platform_driver中的struct device_driver driver中的const struct of_device_id	*of_match_table


struct platform_device中的const char	*name
struct platform_driver中的const struct platform_device_id *id_table中的char name[PLATFORM_NAME_SIZE]


struct platform_device中的const char	*name
struct platform_driver中的struct device_driver driver中的const char  *name

驱动中
struct platform_driver中的struct device_driver driver中的const char  *name和const struct of_device_id	*of_match_table不能为空指针


-C kernel-4.19 O=/home/gonghj/project/mtk8195/out/target/product/xt501v2_64_bsp/obj/KERNEL_OBJ ARCH=arm64 CROSS_COMPILE=aarch64-linux-androidkernel- CLANG_TRIPLE=aarch64-linux-gnu- LD=ld.lld LD_LIBRARY_PATH=/home/gonghj/project/mtk8195/prebuilts/clang/host/linux-x86/clang-r383902/lib64: NM=llvm-nm OBJCOPY=llvm-objcopy CC=clang ROOTDIR=/home/gonghj/project/mtk8195 PATH=/home/gonghj/project/mtk8195/prebuilts/clang/host/linux-x86/clang-r383902/bin:/home/gonghj/project/mtk8195/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin:/usr/bin:/bin:/home/gonghj/project/mtk8195/prebuilts/build-tools/path/linux-x86:/home/gonghj/project/mtk8195/out/.path PROJECT_DTB_NAMES='mediatek/xt501v2_64_bsp' xt501v2_64_bsp_defconfig

make -j24 -C kernel-4.19 O=/home/gonghj/project/mtk8195/out/target/product/xt501v2_64_bsp/obj/KERNEL_OBJ ARCH=arm64 CROSS_COMPILE=aarch64-linux-androidkernel- CLANG_TRIPLE=aarch64-linux-gnu- LD=ld.lld LD_LIBRARY_PATH=/home/gonghj/project/mtk8195/prebuilts/clang/host/linux-x86/clang-r383902/lib64:\$$LD_LIBRARY_PATH NM=llvm-nm OBJCOPY=llvm-objcopy CC=clang ROOTDIR=/home/gonghj/project/mtk8195 PATH=/home/gonghj/project/mtk8195/prebuilts/clang/host/linux-x86/clang-r383902/bin:/home/gonghj/project/mtk8195/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin:/usr/bin:/bin:\$$PATH                                                                                                   PROJECT_DTB_NAMES='mediatek/xt501v2_64_bsp' menuconfig
make -j24 -C kernel-4.19 O=/home/gonghj/project/mtk8195/out/target/product/xt501v2_64_bsp/obj/KERNEL_OBJ ARCH=arm64 CROSS_COMPILE=aarch64-linux-androidkernel- CLANG_TRIPLE=aarch64-linux-gnu- LD=ld.lld LD_LIBRARY_PATH=/home/gonghj/project/mtk8195/prebuilts/clang/host/linux-x86/clang-r383902/lib64: NM=llvm-nm OBJCOPY=llvm-objcopy CC=clang ROOTDIR=/home/gonghj/project/mtk8195 PATH=/home/gonghj/project/mtk8195/prebuilts/clang/host/linux-x86/clang-r383902/bin:/home/gonghj/project/mtk8195/prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin:/usr/bin:/bin:/home/gonghj/project/mtk8195/prebuilts/build-tools/path/linux-x86:/home/gonghj/project/mtk8195/out/.path PROJECT_DTB_NAMES='mediatek/xt501v2_64_bsp'



make  simg2img  lpunpack  unpack_bootimg



