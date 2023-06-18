## 个人信息

- 龚海杰/男/1996/湖南/英语6级
- 手机：17877783755，邮箱：1575454530@qq.com
- 技术博客：https://gitee.com/ghjiee/blog/tree/master/markdown
- 目前在职

## 求职意向

- 期望职位：linux内核网络驱动开发工程师/WiFi开发工程师
- 期望城市：无城市要求

## 教育经历

| 学校         | 学历，专业                     | 时间              |
| ------------ | ------------------------------ | ----------------- |
| 湖南大学     | 全日制专硕，计算机技术         | 2018.09 - 2021.06 |
| 湖南工业大学 | 全日制本科，无机非金属材料工程 | 2014.09 - 2018.06 |


## 技能介绍

* qemu可以模拟的vexpress arm开发板，在qemu里面自己为这款开发板开发了一个虚拟网卡硬件，该虚拟网卡与host的tap设备连接来传输数据。从kernel.org下载一份源码构建内核镜像，使用busybox构建一个根文件系统，用qemu把整个系统跑起来后，接着为guest linux OS开发这个虚拟网卡硬件的网络设备驱动程序。这套虚拟网卡设备及驱动支持如下特性：NAPI、multi queue、DMA ring buffer、 scatter I/O、GRO/GSO、IRQ affinity等。最后借助host NAT转发，在guest里面使用该虚拟网卡成功访问了互联网，交叉编译了iperf工具放进开发板进行了一番性能测试，自己实现的网卡上比开发板原有的虚拟网卡性能要高三倍。项目地址：https://ghjiee.coding.net/p/notes/d/vexpress/git



* 熟悉linux常用命令、linux环境系统编程、网络编程、驱动开发。

* 有mtk/rk/amlogic安卓平台、嵌入式arm linux、stm32、51单片机开发经验。

* 了解linux内核配置/编译、安卓系统构建、安卓系统启动流程，写过Kconfig/Makefile/Android.mk/rc启动脚本。

* 知道安卓binder/hwbinder原理和AIDL/HIDL机制，阅读过相关代码，修改过wpa_supplicant的hidl接口。

* 用过gdb、trace32、ASAN熟悉常用debug手段。

* 研究过安卓平台wifi相关的framework、Android hal、内核驱动代码，对IEEE 802.11和IEEE 802.1x规范标准有一定了解，使用过wifi空口抓包工具。

* 阅读过部分linux网络协议栈源码和中断处理源码，对网卡设备运行机制有一定了解。

  


## 工作经历

**中兴（2021/07 ~ 2022/07）**

- **地点**：湖南长沙
- **职位**：驱动开发工程师(wifi方向)
- **项目描述** : 云电脑终端、云终端、视频会议终端等，涉及mtk/rk/amlogic三个安卓平台。
- **工作内容** : 
  * 负责新产品上wifi功能的适配。曾经独自在刚回板的rk安卓平台上适配过mtk sdio wifi/bt模组的wifi功能，涉及设备树、内核mmc子系统、wifi驱动、wpa_supplciant、android.hardware.wifi@1.x-service等很多地方的修改。
  * 负责wpa_supplicant中IEEE 802.1x、eap认证框架之上，自定义wifi认证协议peap_90的移植与二次开发，以满足通过公司wifi热点接入内网时安全认证的需求。
  * 负责分析/解决各种wifi相关的问题，例如：连接慢/连不上/断连、GBK编码中文热点名兼容、隐藏wifi等问题。

**三星（2022/09 ~ 至今）**

- **地点**：上海
- **职位**：FW软件工程师
- **项目描述** : 基站RU，Xilinx zynq平台。
- **工作内容** : 
  * 负责基站RU软件版本发布，根文件系统镜像制作。
  * linux用户空间驱动移植、修改、测试。
  * 业务软件稳定性支持，GDB/ASAN debug。


## 感谢阅读

感谢您花时间阅读我的简历，期待能与您共事！

<div style="page-break-after: always;"></div>