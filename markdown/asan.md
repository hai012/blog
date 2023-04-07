https://www.codenong.com/47021422/

# 关于gcc：如何为多个C ++二进制文件启用地址清理器


[address-sanitizer](https://www.codenong.com/tag/address-sanitizer/)[gcc](https://www.codenong.com/tag/gcc/)

## How to enable address sanitizer for multiple C++ binaries

我正在开发一种产品，该产品由多个C ++可执行文件和相互依赖的各种库组成。 我正在使用GCC和-fsanitize-address构建它们。
据我了解，如果我想将地址清理器与库一起使用，则必须将其构建为共享对象(这是GCC的默认选项)。 因此，我认为最好的选择是使用-static-libasan静态地为可执行文件构建地址清理器，并为库动态地构建地址清理器。 但是，当我这样做时，在构建C ++可执行文件之一时出现链接错误：

| 1    | ==10823==Your application is linked against incompatible ASan runtimes |
| ---- | ------------------------------------------------------------ |
|      |                                                              |

这使我认为静态和动态版本的地址清除器不能与GCC混合使用，对吗？ 我在消毒剂GitHub页面上找不到有关此的任何信息。

 [相关讨论](javascript:void(0))



- 多数民众赞成在不特定于ASAN，您永远不会做任何图书馆所描述的事情。 您只使用共享版本(用于库和可执行文件，或者仅用于更高版本)，或者仅使用静态版本(仅用于可执行文件)。





------

TLDR：

- 如果您使用GCC / Clang并且主要可执行文件和shlibs都经过了清理，则无需执行任何特殊操作-只需坚持使用默认-fsanitize=address。
- 如果您使用GCC并且只清理了shlib，请再次使用-fsanitize=address并在运行应用程序时另外导出LD_PRELOAD=$(gcc -print-file-name=libasan.so)。
- 如果您使用Clang且仅清理了shlib，请在运行应用程序时使用-fsanitize-address -shared-libasan进行编译/链接，并另外导出LD_PRELOAD=$(clang -print-file-name=libclang_rt.asan-x86_64.so)。

现在一些解释。最初，Asan仅存在于Clang中，默认情况下使用(并仍然使用)-static-libasan。当将其移植到GCC时，GCC开发人员决定首选共享运行时(例如，因为它允许一个人仅清理一个共享库并保留主要的可执行文件而不被清理，例如不重新编译python.exe就清理Python模块，有关其他示例，请参见Wiki)。两种方法都是二进制不兼容的，因此您不能将应用程序的一部分与静态运行时链接，而将一部分与动态运行时链接。

大致

- GCC -fsanitize=address等效于Clangs -fsanitize=address -shared-libasan(而-shared-libasan是Clang中的二等公民，因此未得到很好的支持)
- lang -fsanitize=address等效于GCC -fsanitize=address -static-libasan(同样，-static-libasan是GCC中的二等公民，因此存在一些问题)

附带说明，有关GCC / Clang Asan的其他差异，请参见此有用的Wiki。

 [相关讨论](javascript:void(0))



- 知道了，由于这个问题，我试图用GCC静态编译地址清理器。
- 不需要LD_PRELOAD，在创建库时-fsanitize = address会将libasan.so。*添加为NEEDED，除非您明确要求不要这样做。
- @MarcGlisse是的，但是它不会拦截标准符号，因为它不是库列表中的第一个。 我还隐约记得还有其他重要问题，因此，如果缺少LD_PRELOAD，Asan目前将中止执行。
- @yugr谢谢，多数民众赞成在有用的信息。 我认为作为第一个依赖项的第一个依赖项就足够了(一个间接访问更多)，但显然不是。
- @Perennialista请注意，只要清理了主要可执行文件，您就可以在GCC中使用-static-libasan(只需将可执行文件和shlibs都附加到CFLAGS即可)。 但是请注意这个问题。















运行时，os内存不断减少，最后触发OOM；
触发OOM的原因是free的内存被放在了libasan的隔离区，而这个隔离区默认值太大，导致OS内存耗尽。(查到的默认的隔离区上限为 256M，随着 free内存的增加触发了隔离区的清理，所以先前放在隔离区的内存会被归还给系统)

默认隔离区大小，参考资料：https://zhuanlan.zhihu.com/p/382994002




gcc默认使用libasan.so，clang默认使用libasan.a





gcc 4.9及以上版本支持ASAN



CFLAGS += -g1  -O0
CFLAGS += -fno-common -fsanitize=address  -fno-omit-frame-pointer
LFLAGS += -lasan



查看依赖的libasan.so的路径

gcc --print-file-name libasan.so





ASAN_OPTIONS=detect_invalid_pointer_pairs=1
ASAN_OPTIONS=detect_stack_use_after_return=1
ASAN_OPTIONS=detect_leaks=1
ASAN_OPTIONS=help=1
export LD_PRELOAD=/usr/local/libasan.so

./a.out

export LD_PRELOAD=""

























# 静态链接





CFLAGS += -g1 -Wno-unused-but-set-variable -O1



CFLAGS += -fsanitize=address -fsanitize-recover=address -U_FORTIFY_SOURCE -fno-omit-frame-pointer -fno-common
LFLAGS += -fsanitize=address -static-libasan





 cd /usr/local/app/
 export ASAN_OPTIONS=halt_on_error=0:detect_leaks=1:log_path=/tmp/asan.log:log_exe_name=1
 ./RUSW_APP.out &





-fsanitize=address 开起 asan 能力，gcc 4.8 版本开启支持。
-fsanitize-recover=address ：asan 检查到错误后，不 core 继续运行，需要配合环境变量 ASAN_OPTIONS=halt_on_error=0:report_path=xxx 使用。gcc 6 版本开始支持。





```
cat OutPut/err.txt |grep 'multiple definition of' |grep -v '__odr_asan'|awk '{print $5}'|sort|uniq
```





```
GCC编译选项
-fsanitize=address：开启内存越界检测
-fsanitize=leak：开启内存泄漏检测
-fsanitize-recover=address：一般后台程序为保证稳定性，不能遇到错误就简单退出，而是继续运行，采用该选项支持内存出错之后程序继续运行，需要叠加设置环境变量ASAN_
OPTIONS=halt_on_error=0才会生效；若未设置此选项，则内存出错即报错退出
-fno-stack-protector：去使能栈溢出保护
-fno-omit-frame-pointer：去使能栈溢出保护
-fno-var-tracking：默认选项为-fvar-tracking，会导致运行非常慢
-g1：表示最小调试信息，通常debug版本用-g即-g2



CFLAGS += -fsanitize=address -fsanitize-recover=address -U_FORTIFY_SOURCE -fno-omit-frame-pointer -fno-common
LFLAGS += -fsanitize=address -static-libasan


Asan运行选项
ASAN_OPTIONS是Address-Sanitizier的运行选项环境变量。

halt_on_error=0：检测内存错误后继续运行
detect_leaks=1:使能内存泄露检测
malloc_context_size=15：内存错误发生时，显示的调用栈层数为15
log_path=/var/log/asan.log:内存检查问题日志存放文件路径
suppressions=$SUPP_FILE:屏蔽打印某些内存错误
```

