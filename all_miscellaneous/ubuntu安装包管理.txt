# ubuntu 

apt install  xx   如果失败是因为包依赖的库与已安装库的版本冲突，先apt --fix-broken install，再apt install  xx

dpkg -i xxxx.deb

dpkg -l

apt-get purge xxxx



init进程：ubuntu18后为systemd, 之前的版本有system V init 还有个ubuntu自己的init