根据UNP，linux下判断connect连接成功还有三种方法，用于替代getsockopt：

调用getpeername，如果没错则连接成功
以0长度参数调用read，如果read返回0则连接成功
再次调用connect，如果错误是EISCONN，则表示之前的连接已成功  


1 epoll 模型：将连接的socket加入epoll的监听事件中，然后再调用epoll_wait函
数等待事件触发，如果连接的socket的端口没有监听或者连接的ip不可达都会触发
EPOLLERR事件，socket连接成功一般触发EPOLLOUT事件，但是如果对方在连接成功
后立马给你回一条数据，那么可能同时触发EPOLLIN 和EPOLLOUT事件，网络编程的
书上一般都说连接失败socket会变的可读可写，但是epoll模型中我不知道怎么触
发这种情况，调试没有试出来。

2 select模型：同样将连接的socket加入读写错误三个事件中，调用select函数，
如果socket连接失败那么socket会变的同时可读和可写，如果socket连接成功可
能会出现两种情况，第一是socket可写，第二就是socket同时可读可写。那么现
在问题来了，当socket同时可读可写的时候我们怎么判断socket到底是连接成功
了还是失败了呢。所以我们在socket同时可读可写的时候需要调用另外一个函数
来确定socket是否连接成功，那就是getsockopt函数，getsockopt函数有一个选
项为SO_ERROR，通过该选项我们可以判断出socket是否连接成功，注意不是获取
该函数的返回值而是函数的参数返回值，函数调用中的第四个参数会返回socket
连接错误的错误码，如果成功错误码为0，否则不为0。该函数书上说调用之后会
把错误同时赋值给errno但是我发现没有，只能通过函数调用的参数获取错误。





这里只描述同步Socket的send函数的执行流程。当调用该函数时，send
先比较待发送数据的长度len和套接字s的发送缓冲的 长度，如果len大
于s的发送缓冲区的长度，该函数返回SOCKET_ERROR；如果len小于或者
等于s的发送缓冲区的长度，那么send先检查协议 是否正在发送s的发送
缓冲中的数据，如果是就等待协议把数据发送完，如果协议还没有开始发
送s的发送缓冲中的数据或者s的发送缓冲中没有数据，那么 send就比较s
的发送缓冲区的剩余空间和len，如果len大于剩余空间大小send就一直等
待协议把s的发送缓冲中的数据发送完，如果len小于剩余 空间大小send就
仅仅把buf中的数据copy到剩余空间里（注意并不是send把s的发送缓冲中的
数据传到连接的另一端的，而是协议传的，send仅仅是把buf中的数据copy
到s的发送缓冲区的剩余空间里）。如果send函数copy数据成功，就返回实
际copy的字节数，如果send在copy数据时出现错误，那么send就返回SOCKET_ERROR；
如果send在等待协议传送数据时网络断开的话，那么send函数也返回SOCKET_ERROR。



