#!/bin/bash
time=$[ 60*10 ]
status=1
while [ $status -eq 1 ]
do
    sudo ps aux | grep "./CHttpServer 80" | grep -v grep > /dev/null
    if [ $? -eq 1 ]
    then
        ./CHttpServer 80
    fi
    sleep $time
done
