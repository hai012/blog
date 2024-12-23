## 个人信息

- 龚海杰/男/1996/28岁/籍贯湖南/英语6级
- 手机：17877783755，邮箱：1575454530@qq.com
- 技术笔记博客：   http://e.coding.net/ghjiee/notes/blog.git
- 目前已离职

## 求职意向

- 期望职位：linux驱动开发/linux内核开发
- 期望城市：长沙

## 教育经历

| 学校         | 学历，专业                     | 时间              |
| ------------ | ------------------------------ | ----------------- |
| 湖南大学     | 全日制专硕，计算机技术         | 2018.09 - 2021.06 |
| 湖南工业大学 | 全日制本科，无机非金属材料工程 | 2014.09 - 2018.06 |


## 技能简介

* 熟悉linux常用命令、系统编程、网络编程。
* 从零开发过完整的linux网卡驱动，涉及设备树、中断子系统、linux驱动模型、网络子系统、DMA等。
* 熟悉linux内核配置/编译、根文件系统镜像构建、系统启动流程。
* 用过gdb、trace32、ASAN，ftrace，熟悉常用debug手段。
* 拥有安卓系统、嵌入式arm linux、单片机等开发经验。
* 熟悉常见接口，在裸机/linux环境下做过UART/I2C/SPI/CAN/ethernet相关开发。



## 个人项目

*  qemu虚拟网卡及linux网卡驱动：qemu可以模拟vexpress arm开发板，在qemu里面自己为它设计了一套寄存器实现一个虚拟网卡硬件，寄存器底层通过host的tap设备来传输数据，并在事件到来时根据寄存器配置决定是否触发guest arm CPU中断。从kernel.org下载一份最新长期支持稳定版源码构建内核镜像，使用busybox构建根文件系统，用qemu把整个系统跑起来后，接着为guest linux OS开发这个虚拟网卡硬件的网络设备驱动程序。这套虚拟网卡硬件及驱动支持如下特性：NAPI、multi queue、DMA ring buffer、 scatter/gather I/O、GRO/GSO、IRQ affinity等。最后借助host 对tap网卡NAT转发，在guest里面使用该虚拟网卡成功访问了互联网。交叉编译了iperf2工具放进开发板进行性能测试，开发板原有的虚拟网卡TCP TX极限性能大概80 Mbits/sec，RX极限性能小于10 Mbits/sec且十分不稳定qemu会报错，自己实现的这套虚拟网卡硬件及linux网卡驱动在TCP TX/RX极限性能上均超过300  Mbits/sec。项目地址：https://github.com/hai012/vexpress_virtual_network_card.git



* linux串口网络驱动及esp8266固件开发，全志f1c100s linux + 乐鑫esp8266应该是市面上能找到的最便宜能跑linux并通过wifi网络访问互联网的硬件方案之一了，但遗憾的是乐鑫官方提供的是freeRTOS+lwip这种方式使用esp8266 MCU wifi芯片，并未给esp8266提供linux驱动。于是自己去开发了一套linux驱动+esp8266固件，用杜邦线连上f1c100s核心板和esp8266模块，在linux 中把esp8266 当成网卡去使用。基本解决方案是在linux 网卡驱动中把要发送ip报文通过流控串口传给esp8266，在esp8266中收到后通过lwip ip到mac层的接口发送出去，接收则是在esp8266 lwip mac层到IP层的接口进行拦截，把接收的数据通过串口发给linux网卡驱动，最后再丢给linux网络协议栈。esp8266 在linux host端的串口网络驱动基于tty 、net、irq、gpio等子系统的标准接口实现，因此也能很容易移植到其他带流控串口的linux开发板，只要有流控串口就能通过esp8266访问互联网。项目地址 https://github.com/hai012/esp8266_hosted_linux.git






## 工作经历

**中兴（2021/07 ~ 2022/07）**

- **地点**：湖南长沙
- **职位**：驱动开发工程师
- **项目描述** : 云电脑终端、云终端、视频会议终端等，mtk/rockchip/amlogic安卓平台。
- **工作内容** : 
  * 新产品上wifi功能适配。曾经独自在刚回板的rockchip安卓平台上适配过mtk sdio wifi/bt模组的wifi功能，涉及设备树、内核mmc子系统、wifi驱动、wpa_supplciant、android.hardware.wifi@1.x-service等很多地方的修改，最终能直接在安卓设置界面打开并使用wifi。
  * 负责wpa_supplicant中IEEE 802.1x、eap认证框架之上，自定义wifi认证协议peap_90的移植与二次开发，以满足通过公司wifi热点接入内网时安全认证的需求。
  * 分析/解决各种wifi相关的问题，例如：连接慢/连不上/断连、GBK编码中文热点名兼容等问题。

**三星（2022/09 ~ 2023/07）**

- **地点**：上海
- **职位**：FW软件工程师
- **项目描述** : 基站RU bsp支持，Xilinx zynq 7020/mpsoc平台。
- **工作内容** : 
  * 负责基站RU软件版本发布，根文件系统镜像制作。
  * linux用户空间驱动移植、修改、测试，GDB/ASAN debug。
  * preloader/uboot/linux内核补丁移植适配。
  * 通信稳定性支持，配合FPGA数字团队的同事，开发程序测试用户空间驱动及 i2c/spi 硬件访问各种外设芯片的稳定性。

**吉利（2023/09 ~ 2024/10）**

- **地点**：上海
- **职位**：智能驾驶软件开发岗
- **项目描述** : 黑芝麻A1000b智驾平台bsp开发与维护。
- **工作内容** : 
  * qnx文件系统裁剪，加快上电到业务APP启动的速度。
  * qnx平台相机接口开发，将黑芝麻提供的相机接口封装给吉利中间件软件平台框架，供上层应用使用。
  * 在linux平台上移植黑芝麻的相机驱动。项目组曾决定产品硬件平台不变，把qnx换成linux，产品在相机硬件这块跟黑芝麻的官方参考设计有些不一样，需要移植修改黑芝麻linux移植相机驱动。与另一位同事一起合作，最终10个相机成功出图。
  * uboot相关开发与维护，例如支持secureboot uboot镜像签名 ，为uboot新增通过fastboot刷写norflash的功能，为新批次产品使用的新型号norflash添加u-boot支持。




## 感谢阅读

感谢您花时间阅读我的简历，期待能与您共事！

<div style="page-break-after: always;"></div>