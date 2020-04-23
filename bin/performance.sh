#!/bin/bash
# 性能监控脚本: 
# 1.监控cpu系统负载 2.监控cpu使用率 3.监控交换分区 4.监控磁盘空间

LOG_PATH='/home/ubuntu/OWL/log'

# 监控cpu系统负载
{
    IP=`ifconfig eth0 | grep "inet" | grep "netmask" | grep "broadcast" | sed 's/^[ \t]*//g'  | cut -f 2 -d " "`
    cpu_num=`grep -c 'model name' /proc/cpuinfo`
    count_uptime=`uptime | wc -w`
    load_15=`uptime | awk '{print $'$count_uptime'}'`
    average_load=$(echo ${load_15} ${cpu_num} | awk '{printf("%.2f", $1/$2)}')
    average_int=`echo $average_load | cut -f 1 -d "."`
    if [ $average_int -gt 0 ]
    then
        echo "[$(date +%Y%m%d/%H:%M:%S)] $IP服务器单个核心15分钟的平均负载为 $average_load ,超过警戒值1.0,请立即处理！" >> $LOG_PATH/performance_$(date +%Y%m%d).log
        # WARN TODO
    else
        echo "[$(date +%Y%m%d/%H:%M:%S)] $IP服务器单个核心15分钟的平均负载值为 $average_load ,负载正常" >> $LOG_PATH/performance_$(date +%Y%m%d).log
    fi
}

# 监控cpu使用率
{ 
    cpu_idle=`top -b -n 1 | grep Cpu | awk '{print $8}' | cut -f 1 -d "."`
    if [ $cpu_idle -lt 20 ]
    then
        echo "$IP服务器cpu剩余 $cpu_idle %,使用率已经超过80%,请及时处理." >> $LOG_PATH/performance_$(date +%Y%m%d).log
        #WARN TODO
    else
        echo "$IP服务器cpu剩余 $cpu_idle %,使用率正常" >> $LOG_PATH/performance_$(date +%Y%m%d).log
    fi
} 

#03. 监控交换分区
{
    swap_total=`free -m | grep Swap | awk '{print  $2}'`
    swap_free=`free -m | grep Swap | awk '{print  $4}'`
    swap_used=`free -m | grep Swap | awk '{print  $3}'`

    if [ $swap_used -ne 0 ]
    then
        swap_per=$(echo $swap_free $swap_total | awk '{printf("%.2f", $1 / $2)}')
        swap_warn=0.20
        swap_now=`expr $swap_per \> $swap_warn`
        if [ $swap_now -eq 0 ]
        then
            echo "$IP服务器swap交换分区只剩下 $swap_free M未使用,剩余不足20%,使用率已经超过80%,请及时处理" >> $LOG_PATH/performance_$(date +%Y%m%d).log
            #WARN TODO
        else
            echo "$IP服务器swap交换分区剩下 $swap_free M未使用,使用率正常" >> $LOG_PATH/performance_$(date +%Y%m%d).log
        fi
    else
        echo "$IP服务器交换分区未使用 -1"  >> $LOG_PATH/performance_$(date +%Y%m%d).log
    fi
}

#04. 监控磁盘空间
{
    disk=`df -h | grep -w /dev/vda1 | awk '{print $5}' | cut -f 1 -d "%"`
    if [ $disk -gt 80 ]
    then
        echo "$IP服务器根分区使用率已经超过80%达到 $disk %,请及时处理" >> $LOG_PATH/performance_$(date +%Y%m%d).log
        # WARN TODO
    else
        echo "$IP服务器根分区使用率为 $disk %,使用率正常" >> $LOG_PATH/performance_$(date +%Y%m%d).log
    fi

    #监控登录用户数
    users=`uptime |awk '{print $4}'`
    if [ $users -gt 2 ]
    then
        echo "$IP服务器用户数已经达到 $users 个,请及时处理。" >> $LOG_PATH/performance_$(date +%Y%m%d).log
        #WARN TODO
    else
        echo "$IP服务器当前登录用户为 $users 个,情况正常" >> $LOG_PATH/performance_$(date +%Y%m%d).log
    fi

} 