# Native 内存异常种类

\1. 申请后多次释放 (double free)

\2. 释放后重新使用 (used after free)

\3. 使用越界 (比如申请了50节内存，结果在使用时多用了8字节的内存，这样就把后面的内存的内容踩坏，引起堆结构异常)

\4. 释放时传给free()的地址不是malloc()申请的地址，比如：p = malloc(10); free(p + 5);

\5. 内存泄露：申请内存后，忘记释放或某些释放代码路径没有走到



前面4种内存异常归根到底是使用了非法地址的内存， 可以归纳为内存踩踏， 综上可以把内存异常定义为2类：

\1. 内存踩踏

\2. 内存泄漏



# Native内存泄漏

分析内存泄漏需要俩就每次使用情况并进行监控，下面先介绍下android的内存监控

## **一. 内存使用情况与监测**

**Android/Linux 内存分配重要策略**

Linux 在分配内存时, 为了节省内存, 按需分配, 使用了延时分配以及Copy-On-Write 的策略.

**延时分配(缺页)**

即针对user space 申请memory 时, 先只是明面上的分配虚拟空间, 等到真正操作memory 时, 才真正分配具体的物理内存, 这个需要借助MMU 的data abort 转换成page fault 来达成. 这样就可以极大的避免因user space 过度申请memory, 或者错误申请memory 造成的memory 浪费.

**Copy-On-Write** 

即是在进程fork 时, 子进程和父进程使用同一份memory, 只有当某块memory 被更新时, 才重新copy 出新的一份. 这个在android 上表现也非常显著, 上层app 包括system server 都由zygote fork 出来, 并且没重新exec 新的bin, ART 虚拟机和Lib库 的物理memory 都是共享的，映射到上层app domain自己独立的虚拟地址空间中实现不同进程之间的空间独立, 可以极大的节省Memory 的使用.

 

对应的我们在评估一个进程的memory 使用时, 我们往往就需要观察它使用的虚拟的memory 空间, 它真实的使用的物理memory, 它和其他进程有均摊多少memory, 即:

VSS- Virtual Set Size 虚拟耗用内存（包含共享库占用的内存）

RSS- Resident Set Size 实际使用物理内存（包含共享库占用的内存）

PSS- Proportional Set Size 实际使用的物理内存（比例分配共享库占用的内存）

USS- Unique Set Size 进程独自占用的物理内存（不包含共享库占用的内存）



### **1. 内存的使用情况**

要分析memory leaks, 你需要知道总体的内存使用情况和划分. 以判断内存泄露是发生在user space, kernel space, mulit-media 等使用的memory, 从而进一步去判断具体的memory leaks.

-   user space 使用的memory 即通常包括从进程直接申请的memory, 比如 malloc: 先mmap/sbrk 整体申请大块Memory 后再malloc 细分使用, 比如stack memory, 直接通过mmap 从系统申请; 以及因user space 进程打开文件所使用的page cache, 以及使用ZRAM 压缩 user space memory 存储所占用的memory.
-   kernel space 使用的memory 通常包括 kernel stack, slub, page table, vmalloc, shmem 等.
-   mulit-media 使用的memory 通常使用的方式包括 ion, gpu 等.



其他方式的memory 使用, 此类一般直接从buddy system 中申请出以page 为单位的memory, android 中比较常见如ashmem.



**1.1 整机内存情况：**

**/proc/meminfo**

 从MemAvailable大概判断当前内存使用情况， 从SUnreclaim 可以判断内核释放内存泄漏 ， 从KernelStack 判断线程泄漏， 

1 系统角度观察内存使用情况，首先看/proc/meminfo , 具体字段含义可以参考[common](http://10.234.22.197:6015/source/xref/amlogic/common/)/[Documentation](http://10.234.22.197:6015/source/xref/amlogic/common/Documentation/)/[filesystems](http://10.234.22.197:6015/source/xref/amlogic/common/Documentation/filesystems/)/[proc.txt](http://10.234.22.197:6015/source/xref/amlogic/common/Documentation/filesystems/proc.txt)

```
:/data # cat /proc/meminfo
MemTotal:        3955784 kB    //系统总物理内存（不包含reserved mem 和 kernl code）
MemFree:         2403816 kB    //LowFree+HighFree
MemAvailable:    2721496 kB    //without swapping. Calculated from MemFree, SReclaimable, the size of the file LRU lists, and the low watermarks in each zone.
Buffers:            7904 kB    
Cached:           691320 kB
SwapCached:            0 kB
Active:           466136 kB    //最近使用内存，一般不回收  Active(anon) +  Active(file)
Inactive:         588240 kB    //最近很少被使用的内存，很可能被回收   Inactive(anon) +  Inactive(file)
Active(anon):     357296 kB
Inactive(anon):   167576 kB
Active(file):     108840 kB
Inactive(file):   420664 kB
Unevictable:        2472 kB
Mlocked:            2472 kB
HighTotal:       3227648 kB
HighFree:        1803152 kB
LowTotal:         728136 kB
LowFree:          600664 kB
SwapTotal:        262140 kB
SwapFree:         262140 kB
Dirty:                16 kB
Writeback:             0 kB
AnonPages:        357572 kB
Mapped:           511604 kB
Shmem:            167748 kB
Slab:              57760 kB
SReclaimable:      25580 kB
SUnreclaim:        32180 kB
KernelStack:        8088 kB
PageTables:        21264 kB
NFS_Unstable:          0 kB
Bounce:                0 kB
WritebackTmp:          0 kB
CommitLimit:     2240032 kB
Committed_AS:   27021556 kB
VmallocTotal:     245760 kB
VmallocUsed:           0 kB
VmallocChunk:          0 kB
CmaTotal:         995328 kB
CmaFree:           10872 kB
VmapStack:          4076 kB

对应字段含义截取部分：
    MemTotal: Total usable ram (i.e. physical ram minus a few reserved
              bits and the kernel binary code)
     MemFree: The sum of LowFree+HighFree
MemAvailable: An estimate of how much memory is available for starting new
              applications, without swapping. Calculated from MemFree,
              SReclaimable, the size of the file LRU lists, and the low
              watermarks in each zone.
              The estimate takes into account that the system needs some
              page cache to function well, and that not all reclaimable
              slab will be reclaimable, due to items being in use. The
              impact of those factors will vary from system to system.
     Buffers: Relatively temporary storage for raw disk blocks
              shouldn't get tremendously large (20MB or so)
      Cached: in-memory cache for files read from the disk (the
              pagecache).  Doesn't include SwapCached
  SwapCached: Memory that once was swapped out, is swapped back in but
              still also is in the swapfile (if memory is needed it
              doesn't need to be swapped out AGAIN because it is already
              in the swapfile. This saves I/O)
      Active: Memory that has been used more recently and usually not
              reclaimed unless absolutely necessary.
    Inactive: Memory which has been less recently used.  It is more
              eligible to be reclaimed for other purposes
   HighTotal:
    HighFree: Highmem is all memory above ~860MB of physical memory
              Highmem areas are for use by userspace programs, or
              for the pagecache.  The kernel must use tricks to access
              this memory, making it slower to access than lowmem.
    LowTotal:
     LowFree: Lowmem is memory which can be used for everything that
              highmem can be used for, but it is also available for the
              kernel's use for its own data structures.  Among many
              other things, it is where everything from the Slab is
              allocated.  Bad things happen when you're out of lowmem.
   SwapTotal: total amount of swap space available
    SwapFree: Memory which has been evicted from RAM, and is temporarily
              on the disk
```



**dumpsys meminfo** 

2 从内核角度首先观察/proc/meminfo, 从android 开发人员角度Google 提供了dumpsys meminfo 命令来获取全局以及某个进程的memory 信息

android 在 AativityManagerService 里面提供了一个meminfo 的service , 可以来抓取process 的memory 使用概要, 这个慢慢成为了android 上层判断的主流.

   adb shell dumpsys meminfo  ==> dump 全局的memory 使用情况.

   adb shell dumpsys meminfo pid ==> dump 单个process memory 使用情况.

好处在于, 如果是user build 没有root 权限的时候, 可以借道sh ==> system_server ==> binder ==> process 进行抓取操作, 规避了权限方面的风险

```
:/data # dumpsys meminfo -h
meminfo dump options: [-a] [-d] [-c] [-s] [--oom] [process]
  -a: include all available information for each process.
  -d: include dalvik details.
  -c: dump in a compact machine-parseable representation.
  -s: dump only summary of application memory usage.
  -S: dump also SwapPss.
  --oom: only show processes organized by oom adj.
  --local: only collect details locally, don't call process.
  --package: interpret process arg as package, dumping all
             processes that have loaded that package.
  --checkin: dump data for a checkin
  --proto: dump data to proto
If [process] is specified it can be the name or
pid of a specific process to dump.
```

如下是amlogic dumpsys meminfo 数据中的系统整机内存总结，：

```
Total RAM: 3,955,784K (status normal)
 Free RAM: 2,675,569K (   69,173K cached pss +   205,428K cached kernel + 2,400,968K free)
 Used RAM: 1,187,409K (  860,141K used pss +   327,268K kernel)
 Lost RAM:    92,794K
     ZRAM:        12K physical used for         0K in swap (  262,140K total swap)
   Tuning: 256 (large 512), oom   184,320K, restore limit    61,440K (low-ram)
   

   
与/porc/meminfo对照情况， 通常android使用厂家中都有在Free RAM中都有改动，算法不一致：
Total RAM:  /proc/meminfo-MemTotal
Free RAM:  cached pss = All  pss of process oom_score_adj >= 900
           cached kernel = /proc/meminfo.Buffers + /proc/meminfo.Cached + /proc/meminfo.SlabReclaimable- /proc/meminfo.Mapped
           free = /proc/meminfo.MemFree
Used RAM:  used pss: total pss - cached pss
Kernel: /proc/meminfo.Shmem + /proc/meminfo.SlabUnreclaim + VmallocUsed + /proc/meminfo.PageTables + /proc/meminfo.KernelStack
Lost RAM: /proc/meminfo.memtotal - (totalPss - totalSwapPss) -  /proc/meminfo.memfree - /proc/meminfo.cached - kernel used - zram used
```

**
**

**1.2 进程内存使用情况：**

**dumpsys meminf $(pidof com.zte.vt100)**

针对单个进程 使用dumpsys meminfo 是通过binder 接入app 来抓取. 接到ActivityThread 的 dumpMeminfo 做的统计：

Native heap 从jemalloc取出对应实现是 android_os_Debug_getNativeHeapSize() ==> mallinfo() ==> jemalloc

Dalvik Heap, 使用Runtime 从java heap 取出.
根据process 的smaps 解析数据 Pss Private Dirty Private Clean SwapPss.

```
:/data # dumpsys meminfo $(pidof com.zte.vt100)
Applications Memory Usage (in Kilobytes):
Uptime: 891246 Realtime: 891246

** MEMINFO in pid 4118 [com.zte.vt100] **
                   Pss  Private  Private  SwapPss     Heap     Heap     Heap
                 Total    Dirty    Clean    Dirty     Size    Alloc     Free
                ------   ------   ------   ------   ------   ------   ------
  Native Heap    45759    45696        0        0   342016   339967     2048
  Dalvik Heap     2201     2148        0        0     3427     2571      856
 Dalvik Other      976      976        0        0
        Stack       72       72        0        0
       Ashmem        2        0        0        0
    Other dev   165749   162316       20        0
     .so mmap    10780      476     4440        0
    .jar mmap      644      636        8        0
    .apk mmap     1619        0      292        0
    .ttf mmap       65        0        0        0
    .dex mmap     9760        4     7440        0
    .oat mmap       81        0        0        0
    .art mmap     4860     4504        0        0
   Other mmap     1385        4      464        0
    GL mtrack    30441    30441        0        0
      Unknown     1112     1112        0        0
        TOTAL   275506   248385    12664        0   345443   342538     2904

 App Summary
                       Pss(KB)
                        ------
           Java Heap:     6652    ==》  dalvik heap + .art mmap
         Native Heap:    45696    ==》  Native Heap
                Code:    13296    ==》  .so mmap + .jar mmap + .apk mmap + .ttf mmap + .dex mmap + .oat mmap
               Stack:       72
            Graphics:    30441    ==》  Gfx dev + EGL mtrack + GL mtrack
       Private Other:   164892    ==》  TotalPrivateClean + TotalPrivateDirty - java - native - code - stack - graphics
              System:    14457    ==》  TotalPss - TotalPrivateClean - TotalPrivateDirty

               TOTAL:   275506       TOTAL SWAP PSS:        0

 Objects
               Views:       82         ViewRootImpl:        1
         AppContexts:        5           Activities:        1
              Assets:        2        AssetManagers:        0
       Local Binders:       25        Proxy Binders:       36
       Parcel memory:        7         Parcel count:       28
    Death Recipients:        1      OpenSSL Sockets:        0
            WebViews:        0

 SQL
         MEMORY_USED:       31
  PAGECACHE_OVERFLOW:       19          MALLOC_SIZE:      117

 DATABASES
      pgsz     dbsz   Lookaside(b)          cache  Dbname
         4       24                        2/17/3  /data/user/0/com.zte.vt100/databases/wifi.db
```




**procrank**  

发现系统内存泄漏在usersapce ，如何快速找到内存占用大的进程？ 如何分析系统整体根据进程使用的memory 情况进行排序 ？

```
:/data # procrank -h
Usage: procrank [ -W ] [ -v | -r | -p | -u | -s | -h ]
    -v  Sort by VSS.
    -r  Sort by RSS.
    -p  Sort by PSS.
    -u  Sort by USS.
    -s  Sort by swap.
        (Default sort order is PSS.)
    -R  Reverse sort order (default is descending).
    -c  Only show cached (storage backed) pages
    -C  Only show non-cached (ram/swap backed) pages
    -k  Only show pages collapsed by KSM
    -w  Display statistics for working set only.
    -W  Reset working set of all processes.
    -o  Show and sort by oom score against lowmemorykiller thresholds.
    -h  Display this help screen.


:/data # procrank
  PID       Vss      Rss      Pss      Uss  cmdline
 4109  1879768K  338144K  244539K  230040K  com.zte.vt100
 3663  1244668K  181988K   84384K   72324K  com.android.systemui
 3522  1763560K  192488K   72019K   57132K  system_server
 4035  1240148K  126156K   52997K   46584K  com.zte.videoconf
 3955  1695548K  122484K   35741K   29416K  com.maxhub.screenshare.server
 3291  1571208K  125640K   30294K   20352K  zygote
 4061  1197096K   93356K   26966K   23060K  com.zte.mediactrl
 4078  1146340K   94412K   25555K   22160K  com.zte.vctservice
 4719  1157000K   82988K   16336K   13628K  com.android.settings
 4012  1147596K   80564K   13573K    8596K  com.zte.tr069
 3989  1147500K   82644K   13031K    9528K  com.zte.vcspaudio
 3982  1141940K   76280K   12122K    9080K  com.zte.webserver
 3334   133032K   25736K   11788K    8628K  /system/bin/surfaceflinger
 4802  1128148K   73640K   10976K    8212K  com.zte.update
 3382    55696K   16972K   10109K    9568K  media.codec
......
 5112     5480K    1460K     285K     144K  dropbeard
 4507     5480K    1452K     277K     136K  dropbeard
                           ------   ------  ------
                          869375K  714040K  TOTAL
 RAM: 3955784K total, 2405104K free, 6928K buffers, 690780K cached, 167744K shmem, 57408K slab
```

####  

**从系统角度出发，找到异常进程后， 除了从进程dumpsys meminfo pid 外还可以进一般cat /proc/self/maps smap 获取内存具体映射和使用情况**

**proc/self/maps:**

​    而从进程的角度来讲, 通常情况下进程所使用的memory, 都会通过mmap 映射到进程空间后访问使用(注: 也会一些非常特别异常的流程, 没有mmap 到进程空间), 所以进程的memory maps 资讯是至关重要的. 可以通过cat /proc/self/maps 方式查看

下面枚举一些关键的段:

malloc 通过jemalloc 所管控的空间, 常见的malloc leaks 都会可以看到这种libc_malloc段空间显著增长.

如下是vt100 开机时候libc_malloc的maps 映射关系:

```
address           perms offset  dev    inode               pathname
# cat /proc/4106/maps | grep libc
56b00000-56b80000 rw-p 00000000 00:00 0          [anon:libc_malloc]
57b80000-57c80000 rw-p 00000000 00:00 0          [anon:libc_malloc]
58c80000-58e00000 rw-p 00000000 00:00 0          [anon:libc_malloc]
59380000-5a200000 rw-p 00000000 00:00 0          [anon:libc_malloc]
5a200000-5a280000 rw-p 00000000 00:00 0          [anon:libc_malloc]
5a300000-5a380000 rw-p 00000000 00:00 0          [anon:libc_malloc]
6cc80000-70c80000 rw-p 00000000 00:00 0          [anon:libc_malloc]
79280000-79300000 rw-p 00000000 00:00 0          [anon:libc_malloc]
```

 

lib库映射，第一段 "r-xp" 则是只读并可执行的主体代码段. 第二段 "r--p" 则是这个lib 使用的只读变量段 , 第三段 "rw-p" 则是这个lib 使用的数据段.

```
9045a000-904de000 r-xp 00000000 b3:10 931        /vendor/lib/libc++_shared.so
904de000-904e3000 r--p 00083000 b3:10 931        /vendor/lib/libc++_shared.so
904e3000-904e4000 rw-p 00088000 b3:10 931        /vendor/lib/libc++_shared.so
```



BSS(Block Started by Symbol) 段, 存放进程未初始化的static 以及 gloal 变量, 默认初始化时全部为0. 通常此类不会有memory leaks, 基本上长度在程序启动时就已经决定了.

```
# cat /proc/4106/maps | grep .bss
71e6c000-71e6d000 rw-p 00000000 00:00 0          [anon:.bss]
7246e000-7246f000 rw-p 00000000 00:00 0          [anon:.bss]
727a5000-727a6000 rw-p 00000000 00:00 0          [anon:.bss]
72858000-72859000 rw-p 00000000 00:00 0          [anon:.bss]
72904000-72905000 rw-p 00000000 00:00 0          [anon:.bss]
72a6a000-72a6b000 rw-p 00000000 00:00 0          [anon:.bss]
72bb2000-72bb3000 rw-p 00000000 00:00 0          [anon:.bss]
738b8000-738ba000 rw-p 00000000 00:00 0          [anon:.bss]
74e0a000-74e0b000 rw-p 00000000 00:00 0          [anon:.bss]
750e2000-750e3000 rw-p 00000000 00:00 0          [anon:.bss]
7511f000-75120000 rw-p 00000000 00:00 0          [anon:.bss]
904e4000-904e5000 rw-p 00000000 00:00 0          [anon:.bss]
9127b000-91ac0000 rw-p 00000000 00:00 0          [anon:.bss]
9e622000-9e63b000 rw-p 00000000 00:00 0          [anon:.bss]
a2a19000-a2a1a000 rw-p 00000000 00:00 0          [anon:.bss]
```



Stack , pthread create 时只标注了它底部的 "thread stack guard", 默认pthread stack 大小是1M - 16K. guard 是 4K. 注意的是java thread 在art 里面还会再隔离一个page, 判断收到的SIGSEGV 是否为StackOverflowError.

```
java_stack
95273000-95274000 ---p 00000000 00:00 0          [anon:thread stack guard]
95274000-95275000 ---p 00000000 00:00 0
95275000-95371000 rw-p 00000000 00:00 0

native_stack
91404000-91405000 ---p 00000000 00:00 0          [anon:thread stack guard]
91405000-91502000 rw-p 00000000 00:00 0

Pthread signal stack, 大小为16K,同样底部有guard 保护.
ad97a000-ad97b000 ---p 00000000 00:00 0          [anon:thread signal stack guard]
ad97b000-ad97f000 rw-p 00000000 00:00 0          [anon:thread signal stack]
```



pthread的TLS(THREAD LOCAL STORAGE) 

TLS全称为Thread Local Storage，是系统为解决一个进程中多个线程同时访问全局变量而提供的机制，保证数据的完整性与正确性，也可以称为 Thread Specific Data ，即表面上看起来这是一个全局变量，所有线程都可以使用它，而它的值在每一个线程中又是单独存储的,在linux中主要有pthread_key实现

```
b0cf8000-b0cf9000 ---p 00000000 00:00 0          [anon:bionic TLS guard]
b0cf9000-b0cfc000 rw-p 00000000 00:00 0          [anon:bionic TLS]
```



ashmem 访问/dev/ashmem 然后申请的memory, 通常比较关键是要确认它的name, 一般从它的name 可以得知memory 的申请位置. 至于 (deleted) 标识, 是指 mmap 时有带MAP_FILE flag, 并且对应的path file已经unlink 或者不存在.

```
b0c7c000-b0c7d000 r--s 00000000 00:05 19045      /dev/ashmem/3245fa8c-1a24-4656-afa5-1aa3fdc941ba (deleted)
```



ion ion buffer 的 vma name 标注成dmabuf, 即已经mmap 的ion memory 可以从这个直接统计算出

```
60717000-60f00000 rw-s 00000000 00:0b 14585      anon_inode:dmabuf
```



**proc/self/smaps:**

上面proc/self/maps获取到的只是地址空间信息, 即虚拟地址空间占用情况, 而实际的具体的memory 占用多少需要审查 proc/pid/smaps.

比如上面maps 中的libc_malloc

```
# cat /proc/4106/maps | grep libc
56b00000-56b80000 rw-p 00000000 00:00 0          [anon:libc_malloc]
cat /proc/4109/smaps | grep -C 30 "56b00000-56b80000"
56b00000-56b80000 rw-p 00000000 00:00 0          [anon:libc_malloc]
Name:           [anon:libc_malloc]
Size:                512 kB
Rss:                  84 kB
Pss:                  84 kB
Shared_Clean:          0 kB
Shared_Dirty:          0 kB
Private_Clean:         0 kB
Private_Dirty:        84 kB
Referenced:           84 kB
Anonymous:            84 kB
AnonHugePages:         0 kB
ShmemPmdMapped:        0 kB
Shared_Hugetlb:        0 kB
Private_Hugetlb:       0 kB
Swap:                  0 kB
SwapPss:               0 kB
KernelPageSize:        4 kB
MMUPageSize:           4 kB
Locked:                0 kB
VmFlags: rd wr mr mw me nr
```

这段jemalloc分配的地址空间为512KB, 但是实际使用的RSS=PSS=84KB, 还有大部分地址空间没有填充物理内存



**pmap**

上面的maps和smaps 节点信息很丰富，linux系统提供了pmap 工具直接统计， android 的userdebug版本默认包含工具：

```
:/data # pmap -xq 4109
4109: com.zte.vt100
12c00000    3072    1104    1104       0 rw---  dalvik-main space (region space) (deleted)
12f00000     512       0       0       0 -----  dalvik-main space (region space) (deleted)
12f80000     256     124     124       0 rw---  dalvik-main space (region space) (deleted)
12fc0000     256       0       0       0 -----  dalvik-main space (region space) (deleted)
13000000  520192     568     568       0 rw---  dalvik-main space (region space) (deleted)
56b00000     512     108     108       0 rw---    [anon:libc_malloc]
56bae000    8100       0       0       0 rw-s-  anon_inode:dmabuf
57397000    8100       0       0       0 rw-s-  anon_inode:dmabuf
57b80000    1024     584     584       0 rw---    [anon:libc_malloc]
58497000    8100       0       0       0 rw-s-  anon_inode:dmabuf
58c80000    1536    1432    1432       0 rw---    [anon:libc_malloc]
58f68000       4       0       0       0 -----    [anon:thread stack guard]
58f69000       4       0       0       0 -----    [anon]
.....
.....
bbf63000       4       0       0       0 r----    [vvar]
bbf64000       4       0       0       0 r-x--    [vdso]
ffff0000       4       0       0       0 r-x--    [vectors]
--------  ------  ------  ------  ------
total    1879832  244984  217700       0
```





上面已经介绍如何观察内存使用情况， 单次的内存使用情况并不能说明内存异常点， 需要多次的监控和开始数据对比才能发现内存异常点

### **2. 内存使用情况监控**

内存监测机制可以简单分为两种.

- **轮询**   

--周期性的查看memory 的使用情况, 通常是通过脚本或者daemon 程序周期性的监测. 监控数据根据具体问题而定

eg:

/proc/meminfo 系统总的memory 使用情况.
/proc/zoneinfo 每个zone 的memory 使用情况.
/proc/buddyinfo buddy system 的memory 情况.
/proc/slabinfo slub 的memory 使用分布.
/proc/vmallocinfo vmalloc 的memory 使用情况.
/proc/zraminfo zram 的使用情况, 以及占用memory 情况.
/proc/vmstat 系统memory 根据使用类型的分布.
/sys/kernel/debug/ion/ion_mm_heap mtk multi-media ion memory 使用情况.
/sys/kernel/debug/ion/client_history ion 各个clients 使用的ion 情况粗略统计.
ps -A -T 打印系统所有进程/线程资讯, 可观察每个进程的线程量, 以及VSS/RSS
dumpsys meminfo 从Android 角度观察系统memory 的使用情况.

- **熔断**

即限制memory 使用setrlimit 进行限制, 当到一定程度时, 主动发生异常, 回报错误. 分析问题过程中通常采用轮询监控方案

系统memory leaks , 就会伴随OOM 发生, 严重是直接KE

单个进程 memory leaks, 如果它的oom adj < 0, 即daemon service 或者 persist app, 通常它的memory leaks 也会触发系统OOM , 因为lmk 难以杀掉. 

普通app 发生memory leaks, 则往往直接被LMK 杀掉. 难以对系统产生直接异常. 当然进程也可能无法申请到memory 发生JE, NE 等异常.









## **二. 内存泄漏分析**



平台上的内存问题一直是性能优化和稳定性治理的焦点和痛点，Java 堆内存因为有比较成熟的工具和方法论，加上 hprof快照作为补充，定位和治理都很方便。而 native 内存问题一直缺乏稳定、高效的工具，仅有的 malloc debug不仅性能难以满足需要，还存在 Android 版本兼容的问题。

事实上，native内存泄漏治理一直不乏优秀的工具，已知的可用于调查 native内存泄漏问题的工具主要有：LeakTracer、MTrace、MemWatch、Valgrind-memcheck、TCMalloc、LeakSanitizer

等。由于 Android 平台的特殊性，这些工具要么不兼容，要么接入成本过高，很难在 Android平台上落地。这些工具的原理基本都是：先代理内存分配/释放相关的函数（malloc/calloc/realloc/memalign/free），再通过unwind回溯调用堆栈，最后借助缓存管理过滤出未释放的内存分配记录。因此，这些工具的主要差异也就体现在代理实现、栈回溯和缓存管理三个方面。根据这些工具代理实现的差异：

大致可以分为hook 和 LD_PRELOAD 两大类，典型的如 malloc debug 和 LeakTracer

LD_PRELOAD 机制抢先加载一个定义malloc/calloc/realloc/memalign/free 等同名函数的代理库，这样就全局代理了应用层内存的分配和释放，通过unwind 回溯调用栈并过滤出疑似的内存泄漏信息。Android 平台上的 LD_PRELOAD 是被严格限制的，因为其没有独立的unwind 实现，依赖系统的 unwind 能力;如果把LeakTracer继承到目标so 只能拦截目标so里显示的内存分配和释放，无法拦截其他so 和跨so调用的内存分配和释放；











android native 的内存泄漏可以简单分为如下几种分析：



### malloc 内存泄漏

对于native内存泄漏，比较常见的一类是C堆内存泄漏，即调用malloc申请的内存没有及时释放造成的内存泄漏。cat /proc/pid/maps看到多出很多[anon:libc_malloc]，有可能malloc leak, 通常userspace的内存泄漏98% 都是malloc 使用不当导致的， 重点分析malloc 内存泄漏， 如果代码复杂，需要使用辅助手段 Malloc debug 分析， malloc hook 只能代理malloc/calloc/realloc/free，无法覆盖mmap/mmap64/munmap ， 存在缺陷， 但是目前Android开发中分析native内存泄漏的通用方法；

[【stability】Android Malloc Debug](https://i.zte.com.cn/#/space/5281ec5b761f4c19b7ef3868290cbbe1/wiki/page/9e8875d7a27d4cac861a8ad26365ba31/view) 



### mmap 内存泄漏

Android 对native 的mmap /mmap64内存泄漏分析很不友好没有集成的debug方案；目前通用的方式如下：

mmap映射可以在/proc/self/map_files 中找到, 也可以在maps文件中查找， 找到泄漏的map 地址空间根据大小推测泄漏大小；

```
ls -al   /proc/self/map_files
lrw------- 1 system system 64 2022-01-08 12:20 a796a000-a796e000 -> /dev/ashmem/dalvik-thread local mark stack (deleted)
lrw------- 1 system system 64 2022-01-08 12:20 a7970000-a7972000 -> /dev/ashmem/dalvik-indirect ref table (deleted)
lrw------- 1 system system 64 2022-01-08 12:20 a7972000-a7976000 -> /dev/ashmem/dalvik-thread local mark stack (deleted)
lr-------- 1 system system 64 2022-01-08 12:20 a7976000-a7987000 -> /data/dalvik-cache/arm/system@framework@boot-framework.art
lr-------- 1 system system 64 2022-01-08 12:20 a7987000-a79a7000 -> /dev/__properties__/u:object_r:logd_prop:s0
lr-------- 1 system system 64 2022-01-08 12:20 a79a7000-a79c7000 -> /dev/__properties__/u:object_r:default_prop:s0
lrw------- 1 system system 64 2022-01-08 12:20 a79c9000-a79cd000 -> /dev/ashmem/dalvik-thread local mark stack (deleted)
lr-------- 1 system system 64 2022-01-08 12:20 a79cd000-a79ce000 -> /data/dalvik-cache/arm/system@framework@boot-android.test.base.art
lr-------- 1 system system 64 2022-01-08 12:20 a79ce000-a79cf000 -> /data/dalvik-cache/arm/system@framework@boot-framework-oahl-backward-compatibility.art
lr-------- 1 system system 64 2022-01-08 12:20 a79d0000-a79d1000 -> /data/dalvik-cache/arm/system@framework@boot-android.hidl.manager-V1.0-java.art
lr-------- 1 system system 64 2022-01-08 12:20 a79d1000-a79d2000 -> /data/dalvik-cache/arm/system@framework@boot-android.hidl.base-V1.0-java.art
lr-------- 1 system system 64 2022-01-08 12:20 a79d2000-a79d3000 -> /data/dalvik-cache/arm/system@framework@boot-ims-common.art
lr-------- 1 system system 64 2022-01-08 12:20 a79d3000-a79d4000 -> /data/dalvik-cache/arm/system@framework@boot-voip-common.art
lr-------- 1 system system 64 2022-01-08 12:20 a79d4000-a79d6000 -> /data/dalvik-cache/arm/system@framework@boot-telephony-common.art
lr-------- 1 system system 64 2022-01-08 12:20 a79d7000-a79d8000 -> /data/dalvik-cache/arm/system@framework@boot-ext.art
lr-------- 1 system system 64 2022-01-08 12:20 a79d8000-a79d9000 -> /data/dalvik-cache/arm/system@framework@boot-apache-xml.art
lr-------- 1 system system 64 2022-01-08 12:20 a79da000-a79db000 -> /data/dalvik-cache/arm/system@framework@boot-bouncycastle.art
lr-------- 1 system system 64 2022-01-08 12:20 a79db000-a79de000 -> /data/dalvik-cache/arm/system@framework@boot-core-libart.art
lr-------- 1 system system 64 2022-01-08 12:20 a79de000-a79e3000 -> /data/dalvik-cache/arm/system@framework@boot.art
lr-------- 1 system system 64 2022-01-08 12:20 a79e3000-a79e9000 -> /system/etc/event-log-tags
lr-------- 1 system system 64 2022-01-08 12:20 a79e9000-a79ea000 -> /vendor/framework/droidlogic-tv.jar
lr-------- 1 system system 64 2022-01-08 12:20 a79ef000-a7a0f000 -> /dev/__properties__/u:object_r:exported2_default_prop:s0
lr-------- 1 system system 64 2022-01-08 12:20 a7a0f000-a7a2f000 -> /dev/__properties__/properties_serial
lr-------- 1 system system 64 2022-01-08 12:20 a7a2f000-a7a36000 -> /dev/__properties__/property_info
lr-------- 1 system system 64 2022-01-08 12:20 a7a38000-a7a39000 -> /data/dalvik-cache/arm/system@framework@boot-okhttp.art
lr-------- 1 system system 64 2022-01-08 12:20 a7a39000-a7a3a000 -> /data/dalvik-cache/arm/system@framework@boot-conscrypt.art
lr-------- 1 system system 64 2022-01-08 12:20 a7a3a000-a7a3b000 -> /dev/event-log-tags
lr-------- 1 system system 64 2022-01-08 12:20 a7a43000-a7a44000 -> /vendor/framework/droidlogic.jar
lrw------- 1 system system 64 2022-01-08 12:20 a7a49000-a7a4a000 -> /dev/ashmem/f5c423f7-a8fb-4834-8a89-495fbd8ebe8c (deleted)
lr-------- 1 system system 64 2022-01-08 12:20 a7a4c000-a7a6c000 -> /dev/__properties__/u:object_r:exported_default_prop:s0
lr-------- 1 system system 64 2022-01-08 12:20 a7a6f000-a7a8f000 -> /dev/__properties__/u:object_r:debug_prop:s0
lr-------- 1 system system 64 2022-01-08 12:20 a7a92000-a7ab2000 -> /dev/__properties__/properties_serial
lr-------- 1 system system 64 2022-01-08 12:20 a7ab3000-a7aba000 -> /dev/__properties__/property_info
lr-------- 1 system system 64 2022-01-08 12:20 a7abb000-a7abc000 -> /system/framework/core-oj.jar
lr-------- 1 system system 64 2022-01-08 12:20 a7aca000-a7b89000 -> /system/bin/linker
lr-------- 1 system system 64 2022-01-08 12:20 a7b8a000-a7b90000 -> /system/bin/linker
lr-------- 1 system system 64 2022-01-08 12:20 a7b90000-a7b91000 -> /system/bin/linker
```

mmap的泄漏问题不是很常见，使用比较少可以根据大小和名字代码中分析查找，主动分析mmap的泄漏， andoroid 和linux 中暂时没有现有方法， 可以使用malloc_debug方式 设置mmap hook函数 记录申请释放的backtrace； 目前各个平台厂家中只有mtk平台有libudf 库， 可以使用debug15 分析mmap内存泄漏问题；







### ion 内存泄漏

基本上一块ion内存都有对应的fd，因此ion泄漏伴随着fd泄漏，可以通过查看：

cd /proc/$pid/fd  && ls -al | grep dmabuf

```
:/proc/4113/fd # ls -al | grep dmabuf
lrwx------ 1 system system 64 2022-01-07 17:55 204 -> anon_inode:dmabuf
lrwx------ 1 system system 64 2022-01-07 17:43 206 -> anon_inode:dmabuf
lrwx------ 1 system system 64 2022-01-07 17:43 208 -> anon_inode:dmabuf
```

或者cat /proc/maps中搜索anon_inode:dmabuf关键字，观察是否增长明显判断是否存在ion 内存泄漏

ion内存泄漏通常与平台相关/ion 内存的使用情况 可以在系统 /d/ion 目录中查找使用情况， 不同平台的实现和使用方式不一致，但是都大同小异，不做具体分析

eg ：amlogic 可以在d/ion/heap 和 client 中查看具体进程分配ion内存(amlogic 没有实现client下具体进程统计)

\# cat /d/ion/heaps/cma_ion

​     client       pid       size

\----------------------------------------------------

\----------------------------------------------------

orphaned allocations (info is from last known client):

 composer@2.2-se       3306     8294400 0 1

 composer@2.2-se       3306     8294400 0 1

 composer@2.2-se       3306     8294400 0 1

\----------------------------------------------------

 total orphaned     24883200

​     total     24883200

\----------------------------------------------------

 \# cat /d/ion/heaps/codec_mm_ion

​     client       pid       size

\----------------------------------------------------

\----------------------------------------------------

orphaned allocations (info is from last known client):

\----------------------------------------------------

 total orphaned        0

​     total         0

  deferred free        0

\----------------------------------------------------





### ashmem 内存泄漏

基本一块ashmem内存泄漏,因此ashmem泄漏伴也随着fd泄漏，可以通过查看：

```
:/proc/4113/fd # ls -al | grep ashmem
lrwx------ 1 system system 64 2022-01-07 17:55 205 -> /dev/ashmem
lrwx------ 1 system system 64 2022-01-07 17:55 207 -> /dev/ashmem
lrwx------ 1 system system 64 2022-01-07 17:55 209 -> /dev/ashmem
lrwx------ 1 system system 64 2022-01-07 17:43 42 -> /dev/ashmem
```

另外在/proc/$pid/maps 查找映射的ashmem：

```
/proc/$pid/maps | grep /dev/ashmem
a853f000-a8541000 rw-p 00000000 00:05 21746      /dev/ashmem/dalvik-indirect ref table (deleted)
a8541000-a8543000 rw-p 00000000 00:05 31794      /dev/ashmem/dalvik-indirect ref table (deleted)
a8569000-a856b000 rw-p 00000000 00:05 31793      /dev/ashmem/dalvik-indirect ref table (deleted)
a8570000-a8572000 rw-p 00000000 00:05 31792      /dev/ashmem/dalvik-indirect ref table (deleted)
a858b000-a858f000 rw-p 00000000 00:05 19003      /dev/ashmem/dalvik-thread local mark stack (deleted)
a8596000-a859a000 rw-p 00000000 00:05 19002      /dev/ashmem/dalvik-thread local mark stack (deleted)
a859a000-a859e000 rw-p 00000000 00:05 19001      /dev/ashmem/dalvik-thread local mark stack (deleted)
a859e000-a85a2000 rw-p 00000000 00:05 19000      /dev/ashmem/dalvik-thread local mark stack (deleted)
a85a2000-a85a6000 rw-p 00000000 00:05 18999      /dev/ashmem/dalvik-thread local mark stack (deleted)
a85a6000-a85aa000 rw-p 00000000 00:05 18998      /dev/ashmem/dalvik-thread local mark stack (deleted)
a85aa000-a85ae000 rw-p 00000000 00:05 18997      /dev/ashmem/dalvik-thread local mark stack (deleted)
a85ae000-a85b2000 rw-p 00000000 00:05 18996      /dev/ashmem/dalvik-thread local mark stack (deleted)
a85b2000-a85b6000 rw-p 00000000 00:05 18995      /dev/ashmem/dalvik-thread local mark stack (deleted)
a85b6000-a85ba000 rw-p 00000000 00:05 18994      /dev/ashmem/dalvik-thread local mark stack (deleted)
a85ba000-a85be000 rw-p 00000000 00:05 18993      /dev/ashmem/dalvik-thread local mark stack (deleted)
a85be000-a85c2000 rw-p 00000000 00:05 18992      /dev/ashmem/dalvik-thread local mark stack (deleted)
```

泄漏owner

找到是ashmem泄漏后，需要定位是谁在使用ashmem。

还是看/proc/$pid/mapsashmem后面都会带名字，可以通过这个名字找到使用者：

```
  art/runtime/mem_map.cc
  
  if (use_ashmem) {
    // android_os_Debug.cpp read_mapinfo assumes all ashmem regions associated with the VM are
    // prefixed "dalvik-".
    std::string debug_friendly_name("dalvik-");
    debug_friendly_name += name;
    fd.reset(ashmem_create_region(debug_friendly_name.c_str(), page_aligned_byte_count));

    if (fd.get() == -1) {
      // We failed to create the ashmem region. Print a warning, but continue
      // anyway by creating a true anonymous mmap with an fd of -1. It is
      // better to use an unlabelled anonymous map than to fail to create a
      // map at all.
      PLOG(WARNING) << "ashmem_create_region failed for '" << name << "'";
    } else {
      // We succeeded in creating the ashmem region. Use the created ashmem
      // region as backing for the mmap.
      flags &= ~MAP_ANONYMOUS;
    }
  }
```

下面列出P版本有用到ashmem的owner（搜索代码）：

| **owner**                          | **source**                                                   |
| ---------------------------------- | ------------------------------------------------------------ |
| /dev/ashmem/AshmemAllocator_hidl   | /system/libhidl/transport/allocator/1.0/default/AshmemAllocator.cpp |
| /dev/ashmem/CursorWindow           | /frameworks/base/libs/androidfw/CursorWindow.cpp             |
| /dev/ashmem/MemoryHeapBase         | /frameworks/native/libs/binder/MemoryHeapBase.cpp            |
| /dev/ashmem/AAudioProxyDataBuffer  | /frameworks/av/services/oboeservice/SharedMemoryProxy.cpp    |
| /dev/ashmem/AAudioSharedRingBuffer | /frameworks/av/services/oboeservice/SharedRingBuffer.cpp     |
| /dev/ashmem/gralloc-buffer         | /hardware/libhardware/modules/gralloc/gralloc.cpp            |
| /dev/ashmem/bitmap                 | /frameworks/base/libs/hwui/hwui/Bitmap.cpp                   |
| /dev/ashmem/MessageQueue           | /system/libfmq/include/fmq/MessageQueue.h                    |
| /dev/ashmem/Parcel Blob            | /frameworks/native/libs/binder/Parcel.cpp                    |
| /dev/ashmem/dalvik-                | /art/runtime/mem_map.cc                                      |
| /dev/ashmem/CodeFlinger code cache | /system/core/libpixelflinger/codeflinger/CodeCache.cpp       |



找到使用位置，可以查看对应的代码，然后检查代码逻辑，找出泄漏点，如果代码复杂，则可以在申请ashmem的位置打印调用栈，然后复现问题，分析log看调用栈来推导泄漏点





## 三 ART虚拟机 ：如何让GC同步回收native内存



附：

Raphael 开源地址：

https://github.com/bytedance/memory-leak-detector



xHook 链接：

https://github.com/iqiyi/xHook



xDL 链接：

https://github.com/hexhacking/xDL



Android-Inline-Hook 链接：

https://github.com/ele7enxxh/Android-Inline-Hook



And64InlineHook 链接：

https://github.com/Rprop/And64InlineHook



malloc debug 

链接：[bionic](http://10.234.22.197:6015/source/xref/amlogic/bionic/)/[libc](http://10.234.22.197:6015/source/xref/amlogic/bionic/libc/)/[malloc_debug](http://10.234.22.197:6015/source/xref/amlogic/bionic/libc/malloc_debug/)/[README.md](http://10.234.22.197:6015/source/xref/amlogic/bionic/libc/malloc_debug/README.md)



LeakTracer 链接：

http://www.andreasen.org/LeakTracer/