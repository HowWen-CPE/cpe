#!/bin/sh /etc/rc.common
# hotplug event: $TYPE are required.
BIN=/usr/sbin/ntpclient     
log_msg="NTP-client"

customize_date_and_time()
{
    wantyear="$(nvram show ntp_rule 0 year)"
    wantmon="$(nvram show ntp_rule 0 mon)"
    wantdate="$(nvram show ntp_rule 0 date)"
    wanthour="$(nvram show ntp_rule 0 hour)"
    wantmin="$(nvram show ntp_rule 0 min)"
    wantsec="$(nvram show ntp_rule 0 sec)"
    [ -z "$wantyear" -o -z "$wantmon" -o \
      -z "$wantdate" -o -z "$wanthour" -o \
      -z "$wantmin" -o -z "$wantsec" ] && {
        echo "0"
        return
    }
    datestr="${wantyear}-${wantmon}-${wantdate} $wanthour:$wantmin:$wantsec"
    date -s "$datestr"
    nvram replace attr ntp_rule 0 customized "1"
    echo "$datestr"
}

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
start () {
    type=$(nvram show ntp_rule 0 type)
    local flag=`nvram show ntp_rule 0 custom_time` 
    customize_date_and_time
    echo "$(nvram show ntp_rule 0 zone)" > /etc/TZ
    if [ "$flag" = "1" ];then
    /etc/update_time.sh &
    return "0"
    else
    [ "$type" = "pool" ] && {
        $BIN -c 1 -i 8 -l -s -h $(nvram show ntp_rule 0 pool) || err=1
        [ "$err" = "1" ] && {
            err=0
            $BIN -c 1 -i 8 -l -s -h 220.130.158.71 || err=1
        }
        /etc/update_time.sh &
        [ "$err" = "1" ] && \
            logger -t "ntpclient[54]" "Fail to get current time" || \
            logger -t "ntpclient[55]" "Update date and time $(date)"
        local dnsmasqpid="$(pidof dnsmasq)"
        [ -n "$dnsmasqpid" -a "$err" != "1" ] && {
            kill -ALRM $dnsmasqpid
            echo "kill -ALRM $dnsmasqpid" >> /tmp/12345678
        }
        return $err
    }
    [ "$type" = "ipaddr" ] && {
        $BIN -c 1 -i 8 -l -s -h $(nvram show ntp_rule 0 serv_ipaddr) || err=1
        /etc/update_time.sh &
        [ "$err" = "1" ] && \
            logger -t "ntpclient[61]" "Fail to get current time" || \
            logger -t "ntpclient[62]" "Update date and time $(date)"
        local dnsmasqpid="$(pidof dnsmasq)"
        [ -n "$dnsmasqpid" -a "$err" != "1" ] && {
            kill -ALRM $dnsmasqpid
            echo "kill -ALRM $dnsmasqpid" >> /tmp/12345678
        }
        return  $err
    }
    fi
}

stop () {
    return $err
}

