\1. 安装jdk

sudo apt install openjdk-8-jdk



\2. 安装ctags 一定要使用Universal Ctags， 先卸载exuberant-ctags

\#sudo apt install exuberant-ctags 如果有安装apt-get 源中的ctags，高概率是exuberant ctags，在opengrok索引时候会出错，先卸载：

Universal Ctags (abbreviated as u-ctags) is a maintained implementation of ctags. ctags generates an index (or tag) file of language objects found in source files for programming languages. This index makes it easy for text editors and other tools to locate the indexed items.



Exuberant Ctags (e-ctags) maintained by Darren Hiebert, the ancestor of Universal Ctags, improved traditional ctags with multi-language support, the ability for the user to define new languages searched by regular expressions (called optlib in Universal Ctags), and the ability to generate emacs-style TAGS files. But the activity of the project unfortunately stalled.

```
sudo apt-get remove ctags
sudo apt-get autoremove
```



\3. 下载universal-ctags源码

git clone https://github.com/universal-ctags/ctags.git

![img](opengrok.assets/icon_rar.gif)[ctags-master.zip](https://i.zte.com.cn/udm-doc-501344930103394304)

 

 \4. 编译及安装

缺什么依赖就补什么

 sudo apt-get install autoconf automake libtool

 sudo apt-get install libffi

 sudo apt-get install pkg-config

cd ctags

./autogen.sh 

./configure

make

sudo make install # 出现nothing to be done错误可以忽略



\5. 安装Tomcat

\1) ubuntu自带tomcat8的情况

$ sudo apt-get install tomcat8

安装后会有三个目标：

启动文件:  /usr/share/tomcat8

配置文件:  /etc/tomcat8, 修改端口/etc/tomcat8/confs/server.xml

webapps:  /var/lib/tomcat8

$ sudo /etc/init.d/tomcat8 restart

使用浏览器打开http://localhost:8080 确认是否可访问

\2) ubuntu 20.04没有自带tomcat8

官网https://tomcat.apache.org/download-80.cgi下载tomcat bin压缩包

![img](opengrok.assets/icon_rar.gif)[apache-tomcat-8.5.73.tar.gz](https://i.zte.com.cn/udm-doc-501345136656089088)

修改server.xml中关于监听的端口号

 69   <Connector port="80" protocol="HTTP/1.1"

 70        connectionTimeout="20000"

 71        redirectPort="8443" />

通过./bin/start.sh来启动

验证：‘

服务器本地访问： w3m [http://10.226.44.82](http://10.226.44.82/) 如果可以，证明tomcat服务器启动正常；此时如果外部访问异常，则证明是防火墙问题





\6. 下载解压OpenGrok(使用tomcat8 jdk8 时候opengrok版本不能超过1.5.0)

\# https://oracle.github.io/opengrok/

tar -xvf opengrok-1.3.16.tar.gz 





\7. 复制source.war到Tomcat

\# 可以重命名source，并且链接也会改成对应的名称

cp opengrok-1.3.16/lib/source.war /var/lib/tomcat8/webapps/ 





8 创建目录

 mkdir opengrok/etc opengrok/source opengrok/data



OPENGROK_TOMCAT_BASE=/var/lib/tomcat8

OPENGROK_SRC_ROOT=/home/opengrok/source

cd /home/opengrok/source && ln -s **/mtk/mtk8195 zte_mt8195

OPENGROK_DATA_ROOT=/home/opengrok/data

eport 这些环境变量

echo “export OPENGROK_SRC_ROOT=/home/opengrok/source” >> /etc/profile

echo “export OPENGROK_DATA_ROOT=/home/opengrok/data” >> /etc/profile

echo “export OPENGROK_TOMCAT_BASE=/var/lib/tomcat8” >> /etc/profile

echo  "export OPENGROK_READ_XML_CONFIGURATION=/home/opengrok/etc/configuration.xml"

source /etc/profile



cd opengrok/source && ln -s /home/zte-drive/mtk8195 mtk8195_android_R

检索指定后缀文件：

java -jar /home/opengrok/opengrok-1.3.16/lib/opengrok.jar -P -S -v -s /home/opengrok/source -d /home/opengrok/data -I *.java -I *.c -I *.h -I *.cpp -W /home/opengrok/etc/configuration.xml

检索所有文件：

java -jar /home/opengrok/opengrok-1.3.16/lib/opengrok.jar -P -S -v -s /home/opengrok/source -d /home/opengrok/data -W /home/opengrok/etc/configuration.xml

【使用于44.82的操作说明】

1)创建/etc/profile.d/opengrok.sh

cat /etc/profile.d/opengrok.sh

export OPENGROK_SRC_ROOT=/home2/opengrok/source

export OPENGROK_DATA_ROOT=/home2/opengrok/data

export OPENGROK_TOMCAT_BASE=/home2/opengrok/apache-tomcat-8.5.73

export OPENGROK_READ_XML_CONFIGURATION=/home/opengrok/etc/configuration.xml

2)source /etc/profile

3)生成索引

sudo java -jar /home2/opengrok/opengrok-1.3.16/lib/opengrok.jar -P -S -v -s /home2/opengrok/source -d /home2/opengrok/data -I *.java -I *.c -I *.h -I *.cpp -W /home2/opengrok/etc/configuration.xml



sudo java -jar /home2/opengrok/opengrok-1.3.16/lib/opengrok.jar -P -S -v -s /home2/opengrok/source -d /home2/opengrok/data -W /home2/opengrok/etc/configuration.xml

4)重启服务

sudo /home2/opengrok/apache-tomcat-8.5.73/bin/shutdown.sh

sudo /home2/opengrok/apache-tomcat-8.5.73/bin/startup.sh



\9. 修改实际configuration路径， ：/var/lib/tomcat8/webapps/source/WEB-INF/web.xml

如已设置环境变量可以忽略：export OPENGROK_READ_XML_CONFIGURATION=/home/opengrok/etc/configuration.xml

\# sudo vi /webapps/source/WEB-INF/web.xml 

  <context-param>

​    <description>Full path to the configuration file where OpenGrok can read its configuration</description>

​    <param-name>CONFIGURATION</param-name>

​    <param-value>/home/opengrok/etc/configuration.xml</param-value>

  </context-param>



10 . opengrok 维护

1.添加新代码到/home/opengrok/source 或者软链接到/home/opengrok/source 

cd /home/opengrok/source && ln -s /home/zte-drive/<Add_project> <Add_project>

2.触发建立索引,建议晚上下班执行

sudo java -jar /home/opengrok/opengrok-1.3.16/lib/opengrok.jar -P -S -v -s /home/opengrok/source -d /home/opengrok/data -W /home/opengrok/etc/configuration.xml

3.重启tomcat

 sudo /etc/init.d/tomcat8 restart



\11. 定时同步索引

设置cron定时任务，每周日零点开始以root用户执行/home/opengrok/update.sh

$ cat /etc/crontab

\# /etc/crontab: system-wide crontab

\# Unlike any other crontab you don't have to run the `crontab'

\# command to install the new version when you edit this file

\# and files in /etc/cron.d. These files also have username fields,

\# that none of the other crontabs do.



SHELL=/bin/sh

PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin



\# m h dom mon dow user command

17 *  * * *  root  cd / && run-parts --report /etc/cron.hourly

25 6  * * *  root  test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.daily )

47 6  * * 7  root  test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.weekly )

52 6  1 * *  root  test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.monthly )

0 0  * * 7  root  /home/opengrok/update.sh > /home/opengrok/log.txt

0 12  * * *  /usr/sbin/ntpdate ntpdate  time.zte.com.cn



在/home/opengrok/update.sh文件中首先进入到各个项目源码的顶层目录执行git pull与gerrit里面的代码同步，同步完了后再更新opengrok索引最后重启tomcat

$ cat /home/opengrok/update.sh

\#!/bin/bash

cd /home/opengrok/source/amlogic && su zte-drive -c "git pull"

cd /home/opengrok/source/et302 && su zte-drive -c "git pull"

cd /home/opengrok/source/mtk8195_android_R && su zte-drive -c "git pull"

cd /home/opengrok/source/w100d && su zte-drive -c "git pull"

cd /home/opengrok/source/w600d && su zte-drive -c "git pull"

cd /home/opengrok/source/vcsp && su zte-drive -c "git pull"

java -jar /home/opengrok/opengrok-1.3.16/lib/opengrok.jar -P -S -v -s /home/opengrok/source -d /home/opengrok/data -W /home/opengrok/etc/configuration.xml

/etc/init.d/tomcat8 restart

