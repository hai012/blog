# 1、编译命令
## 1.1配置shell环境并读取产品清单
```
. build/envsetup.sh
```
envsetup.sh中定义了lunch 这个shell函数
## 1.2选择产品及要编译的版本
```
lunch BUILD-BUILDTYPE
```
    执行完毕后会定义一些环境变量，后续make编译时将使用这些环境变量。
    TARGET_PRODUCT=BUILD
## 1.3开始编译
```
make <-jxx>  <otapackage>
```

# make编译时如何包含device/company/product/下的文件？

    make 命令首先调用安卓源码根目录下的Makefile,该Makefile只有三行:
```
    Makefile
        build/make/core/main.mk
            build/make/core/config.mk
                build/make/core/envsetup.mk
                    build/make/core/product_config.mk
                        build/make/core/node_fns.mk
                        build/make/core/product.mk
                        build/make/core/device.mk
```    
    
```
    #文件 build/make/core/product_config.mk：
    all_product_configs := $(get-all-product-makefiles)
```
    #文件   build/make/core/product.mk：
    define get-all-product-makefiles
    $(call get-product-makefiles,$(_find-android-products-files))
    endef
   
    define get-product-makefiles
    $(sort \
    $(eval _COMMON_LUNCH_CHOICES :=) \
    $(foreach f,$(1), \
        $(eval PRODUCT_MAKEFILES :=) \
        $(eval COMMON_LUNCH_CHOICES :=) \
        $(eval LOCAL_DIR := $(patsubst %/,%,$(dir $(f)))) \
        $(eval include $(f)) \
        $(call _validate-common-lunch-choices,$(COMMON_LUNCH_CHOICES),$(PRODUCT_MAKEFILES)) \
        $(eval _COMMON_LUNCH_CHOICES += $(COMMON_LUNCH_CHOICES)) \
        $(PRODUCT_MAKEFILES) \
    ) \
    $(eval PRODUCT_MAKEFILES :=) \
    $(eval LOCAL_DIR :=) \
    $(eval COMMON_LUNCH_CHOICES := $(sort $(_COMMON_LUNCH_CHOICES))) \
    $(eval _COMMON_LUNCH_CHOICES :=) \
    )
    endef

    define _find-android-products-files
    $(file <$(OUT_DIR)/.module_paths/AndroidProducts.mk.list) \
    $(SRC_TARGET_DIR)/product/AndroidProducts.mk
    endef
```
    小结：
    device/company/product/AndroidProducts.mk中需要定义2个变量
    PRODUCT_MAKEFILES
    COMMON_LUNCH_CHOICES
    因此all_product_configs为所有AndroidProducts.mk中PRODUCT_MAKEFILES变量的集合
    all_product_configs consists items like:
    <product_name>:<path_to_the_product_makefile>
    or just <path_to_the_product_makefile> in case the product name is the
    same as the base filename of the product config makefile.


```
    #文件 build/make/core/product_config.mk：
    #接着根据TARGET_PRODUCT从all_product_configs选出current_product_makefile
    current_product_makefile :=
    all_product_makefiles :=
    $(foreach f, $(all_product_configs),\
    $(eval _cpm_words := $(call _decode-product-name,$(f)))\
    $(eval _cpm_word1 := $(word 1,$(_cpm_words)))\
    $(eval _cpm_word2 := $(word 2,$(_cpm_words)))\
    $(eval all_product_makefiles += $(_cpm_word2))\
    $(eval all_named_products += $(_cpm_word1))\
    $(if $(filter $(TARGET_PRODUCT),$(_cpm_word1)),\
        $(eval current_product_makefile += $(_cpm_word2)),))
    _cpm_words :=
    _cpm_word1 :=
    _cpm_word2 :=
    current_product_makefile := $(strip $(current_product_makefile))

    #然后把current_product_makefile包含进来
    $(call import-products, $(current_product_makefile))
```


PRODUCT_PACKAGES  决定构建系统需要构建哪些目标

Android.mk或Android.bp 决定目标如何构建