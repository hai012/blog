
Perf侧重于分析cpu的性能，是linux支持的一个比较古老的工具，rockchip文档中有记录，而且使用比较规范很具有记录意义，Perf对其他功能比如storage GPU DDR等都不是很好用， perf侧重硬件性能配置开发调优，软件性能调优优先使用systrace

```
 perf
usage: perf [‐‐version] [‐‐help] COMMAND [ARGS]
The most commonly used perf commands are:
annotate Read perf.data (created by perf record) and display annotated code
archive Create archive with object files with build‐ids found in perf.data file
bench General framework for benchmark suites
buildid‐cache Manage 
build‐id
 cache.
buildid‐list List the buildids in a perf.data file
diff Read two perf.data files and display the differential profile
inject Filter to augment the events stream with additional information
kmem Tool to trace/measure kernel memory(slab) properties
kvm Tool to trace/measure kvm guest os
list List all symbolic event types
lock Analyze lock events
probe Define new dynamic tracepoints
record Run a command and record its profile into perf.data
report Read perf.data (created by perf record) and display the profile
sched Tool to trace/measure scheduler properties (latencies)
script Read perf.data (created by perf record) and display trace output
stat Run a command and gather performance counter statistics
test Runs sanity tests.
timechart Tool to visualize total system behavior during a workload
top System profiling tool.
See 'perf help COMMAND' for more information on a specific command.
```

其中比较常用的功能有几个： 

record：收集profile数据 

report：根据profile数据生成统计报告 

stat：打印性能计数统计值 

top：cpu占有率实时统计



准备工作：

\1. 首先按Google或芯片厂商的指导，构建一个完整的Android和Kernel的编译环境（如果不关心Kernel可以忽 略）, 这样分析的时候符号表才能匹配上。

\2. 编译Perf

```
~$ . build/envsetup.sh
~$ lunch
~$ mmm external/linux‐tools‐perf
~$ adb root
~$ adb remount
~$ adb push perf /system/bin/
~$ adb shell sync
```

\3. 符号表备份：

符号文件可以简单分为三类： 

a. 平台native代码，这部分代码在编译的过程中会自动生成符号表，不需要我们干预 

b. 平台java代码，对于art虚拟机来说（老版本的dalvik就不说了）最终的编译结果是oat文件，这也是正规的elf文 件，但是默认是不带debug信息。而新版本的Android也提供了自动生成java符号表的工具： 

```
bash art/tools/symbolize.sh
```

c. 第三方apk，如果是来自开源社区，则可以通过修改makefile和套用Android提供的java符号表工具来生成符号 表文件，然后拷贝到Android的符号表目录，注意路径必须要和设备上的完全一致，可以通过showmap来获取设 备上的路径。 如果是商业的apk，基本上已经做过混淆和strip，除非开发商能配合，不然就没招。 

```
~$ adb shell showmap apk_pid
38540 36296 36296 0 0 36216 80 0 3
/data/app/com.android.webview‐2/lib/arm/libwebviewchromium.so
~$ cp libwebviewchromium.so $ANDROID_PRODUCT_OUT/symbols/data/app/com.android.webview‐
2/lib/arm/libwebviewchromium.so
```

\4. 稍微新一点的Android都开起了Kernel的指针保护，这也会影响Perf的record，所以需要临时关闭保护，也可以使用px： 

```
$ adb shell echo 0 > /proc/sys/kernel/kptr_restrict
```



\5. 为了方便分析，一般会把record的数据pull到host端，在host端做分析，所以需要在设备端也安装一下Perf工 具，ubuntu下安装命令如下： 

```
$ sudo apt‐get install linux‐tools‐common
```



\6. 目前大部分的Android平台默认Perf功能都是打开的，所以一般不需要重新配置Kernel，如果碰到Perf被关闭 的情况，可以打开下面几个配置

```
CONFIG_PERF_EVENTS=y
CONFIG_HW_PERF_EVENTS=y
```



\7. 查看硬件平台性能寄存器支持事件

实际上Android移植的Perf还不完整，tracepoint的事件还不支持，例如：block事件，所以如果想要抓去一些内核 子系统的性能信息就无法满足。Android 7.0开始已经去掉了Perf工具，替代它的是Simpleperf 1工具，对 tracepoint的支持比原来的好很多。

```
/data/local # ./perf list
```



\8. 获取系统热点进程

Perf中的top工具可以列出当前cpu的热点，还可以附加Kernel的符号表让信息可方便分析。命令如下：

```
$ adb shell mkdir ‐p /data/local/symbols
$ adb push vmlinux /data/local/symbols/vmlinux
$ adb shell
# perf top ‐‐vmlinux=/path/to/vmlinux ‐d 2
```

perf top还可以只抓取指定进程的pid，这一般是用在要优化某个程序是非常有用，命令如下：

```
perf top ‐‐vmlinux=/path/to/vmlinux ‐d 2 ‐p pid_of_prog
```

perf top还和系统的top一样可以指定刷新间隔2 , 以上命令中的-d选项就是这个功能，单位是秒。



\9. 获取进程统计信息

perf stat用于获取进程某个时间段内的pmu统计信息，命令如下：

\# ./perf stat ‐p 1415

ctrl+c退出，或发信号让Perf进程退出都可以看到统计结果