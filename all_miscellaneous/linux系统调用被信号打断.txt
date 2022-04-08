系统调用或c库函数被信号打断后是否重启取决于：sigaction安装信号时是否打开

action.sa_flags |= SA_RESTART;标志。此外，以下系统调用不会进行重启的系统调用：

 The following interfaces are never restarted after being interrupted
       by a signal handler, regardless of the use of SA_RESTART; they always
       fail with the error EINTR when interrupted by a signal handler:

  * Socket interfaces, when a timeout has been set on the socket
             using setsockopt(2): accept(2), recv(2), recvfrom(2), and
             recvmsg(2), if a receive timeout (SO_RCVTIMEO) has been set;
             connect(2), send(2), sendto(2), and sendmsg(2), if a send
             timeout (SO_SNDTIMEO) has been set.
       
       * Interfaces used to wait for signals: pause(2), sigsuspend(2),
      sigtimedwait(2), and sigwaitinfo(2).
         * File descriptor multiplexing interfaces: epoll_wait(2),
             epoll_pwait(2), poll(2), ppoll(2), select(2), and pselect(2).
  
         * System V IPC interfaces: msgrcv(2), msgsnd(2), semop(2), and
             semtimedop(2).
  
         * Sleep interfaces: clock_nanosleep(2), nanosleep(2), and
             usleep(3).
  
         * read(2) from an inotify(7) file descriptor.
  
         * io_getevents(2).

       The sleep(3) function is also never restarted if interrupted by a
       handler, but gives a success return: the number of seconds remaining
       to sleep.