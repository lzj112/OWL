#!/bin/bash
# 监控重要进程是否在运行
# 生成的进程监控日志 

LOG_PATH='/home/ubuntu/OWL/log'

IP=`ifconfig eth0 | grep "inet" | grep "netmask" | grep "broadcast" | sed 's/^[ \t]*//g'  | cut -f 2 -d " "`

# 想要监控的重要进程
mysql_dir="/usr/sbin/mysqld"
ssh_dir="/usr/sbin/sshd"
nginx_dir="/usr/sbin/nginx"

#进程监控
{ 
for dir in $mysql_dir $ssh_dir $nginx_dir
do
    process_count=$(ps -ef | grep "$dir" | grep -v grep | wc -l)

    for service in mysql ssh nginx
    do
        echo "$dir" | grep -q "$service"
        if [ $? -eq 0 ]
        then
            if [ $process_count -eq 0 ]
            then
                echo "$service is shutdown at $(date +%Y%m%d%H:%M:%S)" >> $LOG_PATH/process_$(date +%Y%m%d).log
                # WARN todo
            else
                echo "$service is running at $(date +%Y%m%d%H:%M:%S)" >> $LOG_PATH/process_$(date +%Y%m%d).log
            fi
        else
            continue
        fi
    done
done
} 