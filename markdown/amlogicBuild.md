# amlogic 安卓bsp 构建

参考https://blog.csdn.net/chenzhen1080/article/details/101000470

## 1、zImage-dtb
CONFIG_BUILD_ARM_APPENDED_DTB_IMAGE 决定是否使用cat 命令直接将dtb附加到zImage后面：

**arch/arm/Makefile:**
```
...省略部分代码...

# Default target when executing plain make
ifeq ($(CONFIG_XIP_KERNEL),y)
KBUILD_IMAGE := xipImage
else ifeq ($(CONFIG_BUILD_ARM_APPENDED_DTB_IMAGE),y)
KBUILD_IMAGE := zImage-dtb
else
KBUILD_IMAGE := zImage
endif

# Build the DT binary blobs if we have OF configured
ifeq ($(CONFIG_USE_OF),y)
KBUILD_DTBS := dtbs
endif

all:    $(KBUILD_IMAGE) $(KBUILD_DTBS)

boot := arch/arm/boot

...省略部分代码...

zImage-dtb: vmlinux scripts dtbs
        $(Q)$(MAKE) $(build)=$(boot) MACHINE=$(MACHINE) $(boot)/$@

...省略部分代码...
```

https://blog.csdn.net/haoyuedangkong_fei/article/details/72728163

build定义 **scripts/Kbuild.include：**
```
build  := -f  $(if $(KBUILD_SRC),$(srctree)/) scripts/Makefile.build obj
```
$(KBUILD_SRC)常规情况下为空，所以的生成zImage-dtb的命令可简化为：
```
make -f  scripts/Makefile.build obj=arch/arm/boot  arch/arm/boot/zImage-dtb
```
根据scripts/Makefile.build中的定义，最终会执行
```
make -C arch/arm/boot arch/arm/boot/zImage-dtb
```

打开**arch/arm/boot/Makefile**

```
...省略部分代码...

include xxxxx

...省略部分代码...

DTB_NAMES := $(subst $\",,$(CONFIG_BUILD_ARM_APPENDED_DTB_IMAGE_NAMES))
ifneq ($(DTB_NAMES),)
DTB_LIST := $(addsuffix .dtb,$(DTB_NAMES))
else
DTB_LIST := $(dtb-y)
endif
DTB_OBJS := $(addprefix $(obj)/dts/,$(DTB_LIST))

ifeq ($(CONFIG_XIP_KERNEL),y)

$(obj)/xipImage: vmlinux FORCE
        $(call if_changed,objcopy)
        @$(kecho) '  Physical Address of xipImage: $(CONFIG_XIP_PHYS_ADDR)'

$(obj)/Image $(obj)/zImage: FORCE
        @echo 'Kernel configured for XIP (CONFIG_XIP_KERNEL=y)'
        @echo 'Only the xipImage target is available in this case'
        @false

else

$(obj)/xipImage: FORCE
        @echo 'Kernel not configured for XIP (CONFIG_XIP_KERNEL!=y)'
        @false

$(obj)/Image: vmlinux FORCE
        $(call if_changed,objcopy)

$(obj)/compressed/vmlinux: $(obj)/Image FORCE
        $(Q)$(MAKE) $(build)=$(obj)/compressed $@

$(obj)/zImage:  $(obj)/compressed/vmlinux FORCE
        $(call if_changed,objcopy)

$(obj)/zImage-dtb:      $(obj)/zImage $(DTB_OBJS) FORCE
        $(call if_changed,cat)
        @echo '  Kernel: $@ is ready'

endif

...省略部分代码...
```
由此可知道如果CONFIG_BUILD_ARM_APPENDED_DTB_IMAGE_NAMES为空，则需要使用dtb-y变量将dts文件加入进来，在前面include的xxxxx文件中决定dtb-y变量的值。
例如在arch/arm/boot/dts/Makefile中定义了
dtb-$(CONFIG_XXX_XXX) += xxxxx.dtb
根据.config文件的CONFIG_XXX_XXX决定是否把arch/arm/boot/dts/下对应的xxxxx.dts最终加入DTB_OBJS变量。

# 2、dtb.img

**device/amlogic/common/factory.mk**

```
...省略部分代码...
INSTALLED_BOARDDTB_TARGET := $(PRODUCT_OUT)/dt.img
...省略部分代码...

$(INSTALLED_BOARDDTB_TARGET) : $(KERNEL_DEVICETREE_SRC) $(DTCTOOL) $(DTIMGTOOL) | $(MINIGZIP)
 	$(foreach aDts, $(KERNEL_DEVICETREE), \
 		sed -i 's/^#include \"partition_.*/#include \"$(TARGET_PARTITION_DTSI)\"/' $(KERNEL_ROOTDIR)/$(KERNEL_DEVICETREE_DIR)/$(strip $(aDts)).dts; \
 		sed -i 's/^#include \"firmware_.*/#include \"$(TARGET_FIRMWARE_DTSI)\"/' $(KERNEL_ROOTDIR)/$(KERNEL_DEVICETREE_DIR)/$(TARGET_PARTITION_DTSI); \
 		if [ -f "$(KERNEL_ROOTDIR)/$(KERNEL_DEVICETREE_DIR)/$(aDts).dtd" ]; then \
 			$(MAKE) -C $(KERNEL_ROOTDIR) O=../$(KERNEL_OUT) ARCH=$(KERNEL_ARCH) CROSS_COMPILE=$(PREFIX_CROSS_COMPILE) $(strip $(aDts)).dtd; \
 		fi;\
		$(MAKE) -C $(KERNEL_ROOTDIR) O=../$(KERNEL_OUT) ARCH=$(KERNEL_ARCH) CROSS_COMPILE=$(PREFIX_CROSS_COMPILE) $(strip $(aDts)).dtb; \
 	)
 	cp device/amlogic/common/common_partition.xml $(PRODUCT_OUT)/emmc_burn.xml
 	./device/amlogic/common/dtsi2xml.sh $(KERNEL_ROOTDIR)/$(KERNEL_DEVICETREE_DIR)/$(TARGET_PARTITION_DTSI)  $(PRODUCT_OUT)/emmc_burn.xml
 ifneq ($(strip $(word 2, $(KERNEL_DEVICETREE)) ),)
 	$(hide) $(DTBTOOL) -o $@ -p $(KERNEL_OUT)/scripts/dtc/ $(KERNEL_OUT)/$(KERNEL_DEVICETREE_DIR)
else# elif dts num == 1
	cp -f $(KERNEL_DEVICETREE_BIN) $@
endif
	if [ -n "$(shell find $@ -size +200k)" ]; then \
		echo "$@ > 200k will be gziped"; \
		mv $@ $@.orig && $(MINIGZIP) -c $@.orig > $@; \
	fi;
	$(hide) $(call aml-secureboot-sign-bin, $@)
	@echo "Instaled $@"
ifeq ($(BOARD_AVB_ENABLE),true)
	cp $@ $(INSTALLED_AVB_DTBIMAGE_TARGET)
	$(AVBTOOL) add_hash_footer \
	  --image $(INSTALLED_AVB_DTBIMAGE_TARGET) \
	  --partition_size $(BOARD_DTBIMAGE_PARTITION_SIZE) \
	  --partition_name dtb
endif
```
由此可知如果KERNEL_DEVICETREE变量中有多个dts文件，则dtb.img由vendor/amlogic/common/tools/dtbTool生成dtbTool将多个dtb文件合成一个，并添加一些header。KERNEL_DEVICETREE在device/amlogic/\$(product)/Kernel.mk中定义。


dtbo位置
common/arch/arm/boot/dts/amlogic/android_p_overlay_dt.dts


缺省只编译arm 32位架构的系统
out/target/product/\$(product)/system  对应  /system.img

out/target/product/\$(product)/root    对应 /

out/target/product/\$(product)/system.img是一个稀疏镜像，转换后得到的system_raw.img挂载后可以看到是一个由out/target/product/\$(product)/root和out/target/product/\$(product)/system组成的镜像。

out/target/product/\$(product)/boot.img 只包含uImage和cmdline

out/target/product/\$(product)/dt.img  可能是单个dtb，也可能是header加多个dtb的组合。

out/target/product/\$(product)/dtbo.img  只包含dtbo