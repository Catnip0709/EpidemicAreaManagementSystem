#!/bin/bash
today=$(date +%Y-%m-%d)
#today="2010-01-30"
dbs=$(mysql EpidemicDatabase -h localhost -u test -Bse "select userID from Quarantine where endDate='$today'");
mysql EpidemicDatabase -h localhost -u test -Bse "delete from Quarantine where endDate='$today'"
if [ $? -eq 0 ]
then
    for userID in $dbs
    do
        mysql EpidemicDatabase -h localhost -u test -Bse "update User set state=\"1\" where userID='$userID'"
        if [ $? -ne 0 ]
        then
            echo "mysql error"
        fi
    done
else
    echo "mysql error"
fi