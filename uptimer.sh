#!/bin/bash

source /etc/rikmail/mailnew.conf

logger "uptimer.sh"

if [ $enable == "true" ]; then

correct=true

divider=0
if [ $minutes != "0" ]; then
  divider=$((divider+minutes/5))
fi
if [ $hours != "0" ]; then
  divider=$((divider+hours*12))
fi
if [ $days != "0" ]; then
  divider=$((divider+days*288))
fi
if [ $days == "0" ] && [ $hours == "0" ] && [ $minutes == "0" ]; then
  logger "incorrect timer format"
fi
logger "divider=$divider"

oncal="*:0/5"

echo "root=localhost" > /etc/ssmtp/ssmtp.conf
echo "mailhub=$smtp:$port" >> /etc/ssmtp/ssmtp.conf
echo "FromLineOverride=YES" >> /etc/ssmtp/ssmtp.conf
echo "AuthUser=$inmail" >> /etc/ssmtp/ssmtp.conf
echo "AuthPass=$inpass" >> /etc/ssmtp/ssmtp.conf
echo "UseTLS=YES" >> /etc/ssmtp/ssmtp.conf


counter=$divider
previnstant=false

echo "counter=$counter" > /etc/rikmail/mailnew.count
echo "previnstant=$previnstant" >> /etc/rikmail/mailnew.count

echo "[Unit]" > /etc/rikmail/rikmail.timer
echo "Description=Logs some system statistics to the systemd journal" >> /etc/rikmail/rikmail.timer
echo "Requires=rikmail.service" >> /etc/rikmail/rikmail.timer
echo >> /etc/rikmail/rikmail.timer
echo "[Timer]" >> /etc/rikmail/rikmail.timer
echo "Unit=rikmail.service" >> /etc/rikmail/rikmail.timer
echo "OnCalendar=$oncal" >> /etc/rikmail/rikmail.timer
echo >> /etc/rikmail/rikmail.timer
echo "[Install]" >> /etc/rikmail/rikmail.timer
echo "WantedBy=timers.target" >> /etc/rikmail/rikmail.timer

sleep 3

cp /etc/rikmail/rikmail.timer /etc/systemd/system/rikmail.timer

sleep 3

systemctl daemon-reload
systemctl restart rikmail.timer

else

logger "uptimer.sh not enabled"

fi

logger "uptimer.sh complete"

