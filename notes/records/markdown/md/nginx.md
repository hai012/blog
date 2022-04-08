ubuntu服务器安装nginx:

apt install nginx

如果init进程为systemd

systemctl      start/stop/restart      nginx

配置文件目录/etc/nginx/nginx.config此文件最终会include  /etc/nginx/sites-avaliable/default

修改default文件的server部分

典型配置：



        server{
    
            listen 80 default_server;
    
            root /var/www/html;
    
            autoindex on;
            #index index.html index.htm index.nginx-debian.html;
            server_name _;
    
            location / {
                    # First attempt to serve request as file, then
                    # as directory, then fall back to displaying a 404.
                    try_files $uri $uri/ =404;
            }
    	}
root用来定位目录，#index index.html index.htm index.nginx-debian.html;为需要传输的文件从左到右匹配

如/var/www/html/index.nginx-debian.html就是刚安装nginx后的welcome的界面