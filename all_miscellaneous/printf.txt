
printf格式控制

%\[flags]\[fldwidth]\[precision]\[lenmodifier]convtype

# flags

flags的可选参数是：

标志                                          说明
‘                            （撇号）将整数按千位分组字符
–                            在字段内左对齐输出

.......未列举完

# fldwidth

fidwidth说明最小字段宽度，转换后参数字符数若小于宽度，则多余字符位置用空格填充。字段宽度是一个非负十进制数，或是一个（*）。
precision说明整型转换后最少输出数字位数、浮点数转换后小数点的最少位数、字符串转换后最大字节数。精度是一个点（.），其后跟随一个可选的非负十进制数或一个星号（*）。

宽度和精度字段两者皆可为*。此时，一个整型参数指定宽度或者精度的值，该整型参数正好位于被转换的参数之前。

# lenmodifier

lenmodifier的可选参数：
长度修饰符                                          说明
hh                                将相应的参数按signed或unsigned char类型输出
h                                  将相应的参数按signed或unsigned short类型输出
l                                   将相应的参数按signed或unsigned long或宽字符类型输出
ll                                  将相应的参数按signed或unsigned long long类型输出
j                                    intmax_t或uintmax_t
z                                   size_t
t                                   ptrdiff_t
L                                  long double

# convtype

convtype的可选参数：
转换类型                             说明
d、i                     有符号十进制
o                           无符号八进制
u                          无符号十进制
x、X                    无符号十六进制
f、F                     双精度浮点数
e、E                    指数格式双精度浮点数
g、G                   根据转换后的值解释为f、F、e或者E
a、A                   十六进制指数格式双精度浮点数
c                          字符（若带长度修饰符l，为宽字符）
s                           字符串（若带长度修饰符l，为宽字符）
p                          指向void的指针
n                        到目前为止，此printf调用的字符的数目将被写入到指针所指向的带符号整形中
%                         一个%字符
C                          宽字符（XSI扩展，等效于lc）
S                          宽字符串（XSI扩展，等效于ls）

