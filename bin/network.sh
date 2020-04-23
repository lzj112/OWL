#!/bin/bash
# linux服务器流量监控监本,监控10s内的上传和下载速率,单位是s

# TODO 加IP

LOG_PATH="/home/ubuntu/OWL/log"
SLEEP_TIME=10

R1=`cat /sys/class/net/eth0/statistics/rx_bytes`
T1=`cat /sys/class/net/eth0/statistics/tx_bytes`

sleep $SLEEP_TIME

R2=`cat /sys/class/net/eth0/statistics/rx_bytes`
T2=`cat /sys/class/net/eth0/statistics/tx_bytes`

TBPS=`expr $T2 - $T1`
RBPS=`expr $R2 - $R1`
TKBPS=`expr $TBPS / 1024`
RKBPS=`expr $RBPS / 1024`

if [ $TKBPS"x" == "x" ]; then 
    TKBPS=0
fi
if [ $RKBPS"x" == "x" ]; then 
    RKBPS=0
fi

IP=`ifconfig eth0 | grep "inet" | grep "netmask" | grep "broadcast" | sed 's/^[ \t]*//g'  | cut -f 2 -d " "`

echo "[$(date +%Y%m%d.%H:%M:%S)] ${IP}: transport= $TKBPS KB/s receive= $RKBPS KB/s" >> ${LOG_PATH}/network_$(date +%Y%m%d).log