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
HOSTSTATE=$(busctl --no-pager call xyz.openbmc_project.State.Host /xyz/openbmc_project/state/host0 org.freedesktop.DBus.Properties Get ss xyz.openbmc_project.State.Host CurrentHostState | cut -c47-)
P105_PCH_AUX=$(busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/P105_PCH_AUX xyz.openbmc_project.Sensor.Value | grep writable | cut -c54-66)
P12V_AUX=$(busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/P12V_AUX xyz.openbmc_project.Sensor.Value | grep writable | cut -c54-66)
P1V8_PCH=$(busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/P1V8_PCH xyz.openbmc_project.Sensor.Value | grep writable | cut -c54-66)
P3V3=$(busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/P3V3 xyz.openbmc_project.Sensor.Value | grep writable | cut -c54-66)
PVCCIN_CPU1=$(busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/PVCCIN_CPU1 xyz.openbmc_project.Sensor.Value | grep writable | cut -c54-66)
PVCCIN_CPU2=$(busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/PVCCIN_CPU2 xyz.openbmc_project.Sensor.Value | grep writable | cut -c54-66)
PVCCIO_CPU1=$(busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/PVCCIO_CPU1 xyz.openbmc_project.Sensor.Value | grep writable | cut -c54-66)
PVCCIO_CPU2=$(busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/PVCCIO_CPU2 xyz.openbmc_project.Sensor.Value | grep writable | cut -c54-66)
PVDQ_ABC_CPU1=$(busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/PVDQ_ABC_CPU1 xyz.openbmc_project.Sensor.Value | grep writable | cut -c54-66)
PVDQ_ABC_CPU2=$(busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/PVDQ_ABC_CPU2 xyz.openbmc_project.Sensor.Value | grep writable | cut -c54-66)
PVDQ_DEF_CPU1=$(busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/PVDQ_DEF_CPU1 xyz.openbmc_project.Sensor.Value | grep writable | cut -c54-66)
PVDQ_DEF_CPU2=$(busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/PVDQ_DEF_CPU2 xyz.openbmc_project.Sensor.Value | grep writable | cut -c54-66)
PVNN_PCH_AUX=$(busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/PVNN_PCH_AUX xyz.openbmc_project.Sensor.Value | grep writable | cut -c54-66)

echo $from  > /etc/rikmail/mail.txt
echo $to  >> /etc/rikmail/mail.txt
echo $subj  >> /etc/rikmail/mail.txt
echo >> /etc/rikmail/mail.txt
echo $body1  >> /etc/rikmail/mail.txt
echo "HOST STATE = $HOSTSTATE" >> /etc/rikmail/mail.txt
echo "P105_PCH_AUX = $P105_PCH_AUX" >> /etc/rikmail/mail.txt
echo "P12V_AUX = $P12V_AUX" >> /etc/rikmail/mail.txt
echo "P1V8_PCH = $P1V8_PCH" >> /etc/rikmail/mail.txt
echo "P3V3 = $P3V3" >> /etc/rikmail/mail.txt
echo "PVCCIN_CPU1 = $PVCCIN_CPU1" >> /etc/rikmail/mail.txt
echo "PVCCIN_CPU2 = $PVCCIN_CPU2" >> /etc/rikmail/mail.txt
echo "PVCCIO_CPU1 = $PVCCIO_CPU1" >> /etc/rikmail/mail.txt
echo "PVCCIO_CPU = $PVCCIO_CPU2" >> /etc/rikmail/mail.txt
echo "PVDQ_ABC_CPU1 = $PVDQ_ABC_CPU1" >> /etc/rikmail/mail.txt
echo "PVDQ_ABC_CPU2 = $PVDQ_ABC_CPU2" >> /etc/rikmail/mail.txt
echo "PVDQ_DEF_CPU1 = $PVDQ_DEF_CPU1" >> /etc/rikmail/mail.txt
echo "PVDQ_DEF_CPU2 = $PVDQ_DEF_CPU2" >> /etc/rikmail/mail.txt
echo "PVNN_PCH_AUX = $PVNN_PCH_AUX" >> /etc/rikmail/mail.txt
echo $body2  >> /etc/rikmail/mail.txt
#echo $body3  >> /etc/rikmail/mail.txt
echo >> /etc/rikmail/mail.txt


if [ $recipient != "info@example.com" ]; then
sleep 5
/usr/sbin/postfix start
sleep 10
/usr/sbin/sendmail -t < /etc/rikmail/mail.txt
sleep 10
/usr/sbin/postfix stop
sleep 10
fi


