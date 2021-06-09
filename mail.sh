#!/bin/bash

params=$(tr '_' ';|' < /etc/rikmail/rikmail.conf) 
arr=(${params//;/ })
mode=${arr[0]}
period=${arr[1]}
recipient=${arr[2]}

oncal="*:0/2"

if [ $period -eq 1 ]; then
oncal="*:0/2"
elif [ $period -eq 2 ]; then
oncal="daily"
fi

unit1="[Unit]"
unit2="Description=Logs some system statistics to the systemd journal"
unit3="Requires=rikmail.service"
timer1="[Timer]"
timer2="Unit=rikmail.service"
timer3="OnCalendar=$oncal"
install1="[Install]"
install2="WantedBy=timers.target"

echo $unit1 > /etc/rikmail/rikmail.timer
echo $unit2 >> /etc/rikmail/rikmail.timer
echo $unit3 >> /etc/rikmail/rikmail.timer
echo >> /etc/rikmail/rikmail.timer
echo $timer1 >> /etc/rikmail/rikmail.timer
echo $timer2 >> /etc/rikmail/rikmail.timer
echo $timer3 >> /etc/rikmail/rikmail.timer
echo >> /etc/rikmail/rikmail.timer
echo $install1 >> /etc/rikmail/rikmail.timer
echo $install2 >> /etc/rikmail/rikmail.timer

sleep 3

cp /etc/rikmail/rikmail.timer /etc/systemd/system/rikmail.timer

sleep 3

from="From: \"Rikor-Scalable EATX Board\" <Rikor-Scalable@rikor.com>"
to="To: \"Administrator\" <$recipient>"
subj="Subject: This is a test"
body1="Hi $recipient,"
body2="This is a test. Mode: $mode. Period: $period"
body3="Bye!"

echo $from  > /etc/rikmail/mail.txt
echo $to  >> /etc/rikmail/mail.txt
echo $subj  >> /etc/rikmail/mail.txt
echo >> /etc/rikmail/mail.txt
echo $body1  >> /etc/rikmail/mail.txt
echo $body2  >> /etc/rikmail/mail.txt
echo $body3  >> /etc/rikmail/mail.txt
echo >> /etc/rikmail/mail.txt

sleep 5

/usr/sbin/postfix start
sleep 10
/usr/sbin/sendmail -t < /etc/rikmail/mail.txt
sleep 10
/usr/sbin/postfix stop
sleep 10


