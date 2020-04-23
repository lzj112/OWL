#!/bin/bash
#分析流量监控日志,统计出当日最大上传和下载速度

LOG_PATH="/home/ubuntu/OWL/log"

TX=0
RX=0
MAX_TX=0
MAX_RX=0
while read line
do
    a=`echo $line | grep "transport" |awk '{print $4}'`
    if [ $a -ge 0 ]
    then
        TX=$a
        if [ $TX -ge $MAX_TX ]
        then
            MAX_TX=$TX
        fi
    fi
    b=`echo $line | grep "receive" |awk '{print $7}'`
    if [ $b -ge 0 ]
    then
        RX=$b
        if [ $RX -ge $MAX_RX ]
        then
            MAX_RX=$RX
        fi
    fi
done < ${LOG_PATH}/network_$(date +%Y%m%d).log

echo "[$(date +%Y%m%d.%H:%M:%S)] max_transport_speed $MAX_TX kb/s" >> ${LOG_PATH}/statistical_$(date +%Y%m%d).log

echo "[$(date +%Y%m%d.%H:%M:%S)] max_receive_speed $MAX_RX kb/s" >> ${LOG_PATH}/statistical_$(date +%Y%m%d).log
