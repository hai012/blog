
http://blog.chinaunix.net/uid-28541347-id-4288802.html


epoll_wait返回后事件并没有从epollFd中清除



边缘触发时如果数据没读完需要再次读有2种解决方案让epoll_wait再次返回然后再次读取：
1、等待下次数据到来再次变得可读
2、利用epoll_ctl  EPOLL_CTL_MOD  重新注册

