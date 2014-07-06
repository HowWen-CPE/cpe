#!/bin/sh
# update_time.sh used to update when time_zone has changed by busybox utilities date.c
set -x
sleep 3
updateTime()
{
    time=$(date -Iseconds)
    year=`echo ${time:0:4}`
    month=`echo ${time:5:2}`
    day=`echo ${time:8:2}`
    hour=`echo ${time:11:2}`
    minute=`echo ${time:14:2}`
    second=`echo ${time:17:2}`
    datestr="${year}-${month}-${day} $hour:$minute:$second"

    nvram replace attr ntp_rule 0 year $year
    nvram replace attr ntp_rule 0 mon $month
    nvram replace attr ntp_rule 0 date $day
    nvram replace attr ntp_rule 0 hour $hour
    nvram replace attr ntp_rule 0 min $minute
    nvram replace attr ntp_rule 0 sec $second
    echo "$datestr"
    nvram commit
}
updateTime
set +x