#!/bin/bash
today=$(date +%Y-%m-%d)
yesterday=$(date -d yesterday +%Y-%m-%d)
status=(2 3 4 5)
todayCount=(0 0 0 0)
increaseCount=(0 0 0 0)
lastDayCount=$(mysql EpidemicDatabase -h localhost -u test -Bse "select segregation,suspected,Suspected,die from SpecialStateDailyRecord where date='$yesterday'")
if [ $? -ne 0 ]
then
    echo "mysql error"
fi
i=0
for num in $lastDayCount
do
    count=$(mysql EpidemicDatabase -h localhost -u test -Bse "select count(*) from User where state='${status[$i]}'")
    if [ $? -ne 0 ]
    then
        echo "mysql error"
        break;
    fi
    todayCount[i]=$count
    increaseCount[i]=$[ $count-$num ]
    i=$[ $i+1 ]
done
mysql EpidemicDatabase -h localhost -u test -Bse "insert into SpecialStateDailyRecord values (\"$today\",${todayCount[0]},${todayCount[1]},${todayCount[2]},${todayCount[3]},${increaseCount[0]},${increaseCount[1]},${increaseCount[2]},${increaseCount[3]})";
if [ $? -ne 0 ]
then
    echo "mysql error"
fi