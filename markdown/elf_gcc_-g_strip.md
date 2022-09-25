

gcc -Og -g



gcc不加-g进行编译出可执行elf，然后再使用strip可执行elf时默认会把elf文件中的如下两个段干掉:
  [26] .symtab
  [27] .strtab




gcc 加入-g选项编译出可执行elf后会多出很多debug_xxx段，使用strip可执行elf时默认会把elf文件中的如下7个段干掉:

  [26] .debug_aranges
  [27] .debug_info
  [28] .debug_abbrev
  [29] .debug_line
  [30] .debug_str
  [31] .symtab
  [32] .strtab


debug_xxx段： 编译时加入-g选项生成的调试信息所在段
dynamic  动态链接信息
symtab  符号表(包含dynamic )
strtab    保存elf文件中一些名字的字符串，例如段名



安卓 lunch xxx-userdebug后编译出的native elf在如下路径
out/target/product/tb8185p2_64/obj/EXECUTABLES/wpa_cli_intermediates/LINKED/wpa_cli，这个elf文件默认带有使用-g选项编译出的debug_xxx段，但out/target/product/tb8185p2_64/obj/EXECUTABLES/wpa_cli_intermediates/wpa_cli
文件被strip了，导致实际上打包进镜像的没有debug_xxx段和符号表。
mk编译模块时默认进行了strip，加入如下可以取消strip：
LOCAL_STRIP_MODULE := false






一、文件头：
1.ELF魔数、文件机器字节长度、数据存储方式、版本、运行平台、ABI版本、ELF重定位类型、硬件平台、硬件平台版本、入口地址、程序头入口和长度、段表的位置和长度及段的数量等。

二、段表：
1.保存段的基本特性，比如每个段的段名、段的长度、在文件中的偏移、读写权限及段的其他属性。
2.ELF文件的段结构就是由段表决定的，编译器、链接器和装载器都是依靠段表来定位和访问各个段的属性的。

三、代码段（.text）
1、代码转成的汇编指令

四、数据段
1、(.data)里放已经初始化了的全局静态变量和局部静态变量，有时候编译器会把字符串常量放到(.data)段
2、(.rodata)里放的是只读数据，一般是程序里的只读变量(如const修饰的变量)和字符串常量。

五、BSS段
1、(.bss)段里放的是未初始化的全局变量和局部静态变量，有的编译器会将全局的未初始化变量存放在目标文件.bss段，有些则不放，只是预留一个未定义的全局变量符号，等到最终链接成可执行文件的时候再在.bss段分配空间。
六、(.strtab)段
1、字符串表，用于存储ELF文件中用到的各种字符串

七、(.symtab)段
1、符号表

八、(.shstrtab)段
1、段名表

九、(.comment)段
1、存放的是编辑器信息版本信息

十、(.dynamic)段
1、动态链接信息

十一、(.plt .got)段
1、动态链接的跳转表和全局入口表

十二、(.init .fini)段
1、程序初始化与终结代码段
十三、(.rel.*)
1、重定位表，链接的时候需要对目标文件中某些不为进行重定位，即代码段和数据段中那些对绝对地址的引用的位置。