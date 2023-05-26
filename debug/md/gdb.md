```
//gdb 附着在已经运行的程序上
gdb attach ${PID}
//gdb 调试某个可执行文件
gdb ${executable.elf}

//开始从_start即elf文件定义的程序入口地址执行程序，执行到main函数入口时停止
start

//开始从_start即elf文件定义的程序入口地址执行程序，执行到断点时停止
r

//反汇编某个函数,并显示对应的文件以及行数以及对应的汇编代码
disass /s func_name

//打断点：
b func_name
b Test/BoardTest.c:2579


//直接调用某个函数
call TestCpriLoopback(1)

//当遇到断点停止后，再继续执行直到遇到下一个断点
c

//查看变量的值，如果查看函数内的局部变量需在函数内打好断点，执行到断点后再执行p命令查看
p ${var_name}
```

