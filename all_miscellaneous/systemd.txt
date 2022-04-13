http://www.ruanyifeng.com/blog/2016/03/systemd-tutorial-commands.html
# 列出正在运行的 Unit
$ systemctl list-units

# 列出所有Unit，包括没有找到配置文件的或者启动失败的
$ systemctl list-units --all

# 列出所有没有运行的 Unit
$ systemctl list-units --all --state=inactive

# 列出所有加载失败的 Unit
$ systemctl list-units --failed

# 列出所有正在运行的、类型为 service 的 Unit
$ systemctl list-units --type=service



配置文件相关
systemctl list-unit-files --all --type=service  | grep enabled



/etc/systemd/system.conf
DefaultTasksMax=infinity

systemd检测/etc/systemd/system/下的符号链接决定是否启用该service或target
/etc/systemd/system/xxxxx.service符号链接指向/lib/systemd/system/xxxxx.service


# 查看当前系统的所有 Target
$ systemctl list-unit-files --type=target

# 查看一个 Target 包含的所有 Unit
$ systemctl list-dependencies multi-user.target

# 查看启动时的默认 Target
$ systemctl get-default

# 设置启动时的默认 Target
$ sudo systemctl set-default multi-user.target

# 切换 Target 时，默认不关闭前一个 Target 启动的进程，
# systemctl isolate 命令改变这种行为，
# 关闭前一个 Target 里面所有不属于后一个 Target 的进程
$ sudo systemctl isolate multi-user.target

