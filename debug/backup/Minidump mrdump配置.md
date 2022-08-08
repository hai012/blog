\1. 为方便后期调试debug，适配minidump /mrdump

sysrq-trigger 触发的minidump中数据异常，ddr 数据有丢失



\2. 打开mrdump 编译错误，解决编译问题，测试发现无法抓到mrdump；

ARCH_HAVE_MT_RAMDUMP := yes

MTK_MRDUMP_ENABLE := yes

  userspace Coredump打开条件：

```
	setprop ro.vendor.aee.enforcing no
	setprop persist.vendor.aeev.core.dump enable
	setprop persist.vendor.aeev.core.direct enable
	setprop persist.vendor.mtk.aee.mode 3
	setprop persist.vendor.mtk.aeev.mode 3
	setprop persist.vendor.mtk.aee.filter 0
	setprop persist.vendor.anr.dumpthr 1
```



\3. debug发现 DDR reserve 没有enable ， 这也是minidump ddr 数据丢失和mrdump 无法抓取的原因，mt8195 没有CONFIG_MTK_CONFIG相关配置代码暂时无法打开mrdump

mt8185 可以使能mrdump； 

开启ramdump功能，kernel发生crash后会在lk生成ramdump，由于lk没有文件系统的概念，无法在EXT4文件系统里创建文件并将ramdump保存起来，目前的做法是在kernel阶段先将文件创建好/data/vendor/dumpsys/mrdump_preallocated），然后将文件的位置（lbaooo）再传给lk， ramdump是压缩过的，大小随内容变化而变化，因此该文件要预留多大的空间很重要，那如何调整该文件的大小呢？

1 可通过改code直接关闭MRDUMP feature：

  在lk中：platform/mtxxx/rules.mk中：set MTK_MRDUMP_ENABLE=no

2 如果有root权限，可使用如下命令直接关闭：

  mrdump_tool output-set none

3 如果有root权限，可设置size：

  mrdump_tool file-allocate 0/1/2       0/1/2分别代表：关，hafmem，fullmem.



开机后确认lbaooo参数不为0 ，才能确认lk dump保存位置；

cat sys/modules/mrdump/parameter/lbaooo/





















\--------------------------------------------------------------------

minidump：

do ~~/t32.cmm

sys.res

sys.cpu CortexA55

sys.config.cn 8.

core.num 8.

sys.o mmuspaces on

sys.u

title os.pwd()



d.load.e symbols/vmlinux 0x0000009B5EE00000 /nc /ny

d.load.e symbols/vmlinux 0x0000001B5EE00000 /nc /nocode  [minidump 需要把nocde移除掉，否则用的是load.data中数据看不到汇编]

y.d _end

d.load.e SYS_MINI_RDUMP 0x0000008000000000 /nc /ny /logload

d.load.b scrutils/cmmmmu.bin 0x0000008000000000 /b /nc

global &MemMapAddr &ModStart &LinearStart

&MemMapAddr=0x0000000000000000

&ModStart=0xFFFFFF8000000000

&LinearStart=0xFFFFFFFDC0000000



core.select 0

r.s sp_el0	0xFFFFFFFF1B260E80

r.s cpsr	0x0000000080000089

r.s pc	0xFFFFFF9B6761A560

r.s sp 0xFFFFFF801AB93A40

r.s x0	0x0000000000000002

r.s x1	0x0000000000000000

r.s x2	0xFFFFFF9B68614B85

r.s x3	0x00000000000016F1

r.s x4	0x0000024178C142F1

r.s x5	0x00000000000016F1

r.s x6	0xFFFFFF9B6869CE45

r.s x7	0x6970706F7473203A

r.s x8	0xFFFFFF801AB93A48

r.s x9	0x00000000AEEDEAD2

r.s x10	0x0000000006666998

r.s x11	0x0000000000000001

r.s x12	0xFFFFFF9AF6F745EC

r.s x13	0x0000000000000000

r.s x14	0x0000000000000050

r.s x15	0xFFFFFF9B67F17B2F

r.s x16	0xFFFFFF9B6763DD4C

r.s x17	0x0000000000000000

r.s x18	0x0000000000000090

r.s x19	0xFFFFFF9B68614B85

r.s x20	0x0000000000000000

r.s x21	0x0000000000000000

r.s x22	0xFFFFFF9B6846DB58

r.s x23	0x0000000000000004

r.s x24	0xFFFFFF9B684DF618

r.s x25	0x0000000056000000

r.s x26	0x0000000000000000

r.s x27	0x0000000000000000

r.s x28	0xFFFFFFFF1B260E80

r.s x29	0xFFFFFF801AB93B90

r.s x30	0xFFFFFF9B6761A4F8

per.s spr:0x30201 %q 0x0000008000000000

per.s spr:0x30202 %q 0x000001B2B5593519

per.s spr:0x30100 %l 0x3465591D

per.s spr:0x30A20 %q 0x0000BBFF440C0400

per.s spr:0x30A30 %q 0x0000000000000000

mmu.format linuxswap3 A:0x0000008000000000

trans.common 0xFFFFFF8000000000--0xFFFFFFFFFFFFFFFF

trans.tw on

trans.on





Data.LOAD.e C:\Users\10312024\Desktop\vmlinux /nc /nocode

Data.LOAD.e path\dameon.ko /codesec /reloc .text at 0x0000006E1b100B34 /nc /ny

Data.LOAD.e path\dameon.ko /codesec /reloc .text at 0xFFFFFFEE1b100B34 /nc /ncode



Data.LOAD E:\BUG\xt7\WCXT702-core\vct_main 

Data.LOAD E:\BUG\xt7\WCXT702-core\core-H323-2807-1614823492 /nc /ny



32位系统 ： d.load.e PROCESS_COREDUMP /nc /ny 异常可以用  d.load.binary 试试

if os.file("PROCESS_COREDUMP")

  d.load.binary PROCESS_COREDUMP /nc /ny



set osabi GNU/Linux

show osabi

set auto-solib-add off

set print thread-events off

set print pretty on

dir src

set solib-search-path symbols;symbols/system/lib64

set sysroot symbols

file symbols/surfaceflinger

core PROCESS_COREDUMP

info proc mappings  ==> show maps

sharedlibrary libc.so

sharedlibrary libc++.so

sharedlibrary libsurfaceflinger.so

sharedlibrary libutils.so

sharedlibrary libhidlbase.so

sharedlibrary libbinder.so

sharedlibrary surfaceflinger

sharedlibrary linker64

info sharedlibrary ==> show which syms already

info reg =>show register

info all-reg    ==> include dsh



source script  ==>read, execute GDB commands from ﬁle script