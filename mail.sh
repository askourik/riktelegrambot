#!/bin/bash

function Voltages {
xyzADCSensor="xyz.openbmc_project.ADCSensor"
xyz_voltage="/xyz/openbmc_project/sensors/voltage/$1"
xyzSensorCritical="xyz.openbmc_project.Sensor.Threshold.Critical"
xyzSensorWarning="xyz.openbmc_project.Sensor.Threshold.Warning"
xyzSensorValue="xyz.openbmc_project.Sensor.Value"
introspect="busctl --system introspect"

volterr="$introspect $xyzADCSensor $xyz_voltage $xyzSensorCritical"
voltwar="$introspect $xyzADCSensor $xyz_voltage $xyzSensorWarning"
voltval="$introspect $xyzADCSensor $xyz_voltage $xyzSensorValue"

CriticalAlarmHigh=$($volterr | awk '/\.CriticalAlarmHigh/ {print $4}')
CriticalAlarmLow=$($volterr | awk '/\.CriticalAlarmLow/ {print $4}')
CriticalHigh=$($volterr | awk '/\.CriticalHigh/ {print $4}')
CriticalLow=$($volterr | awk '/\.CriticalLow/ {print $4}')
WarningAlarmHigh=$($voltwar | awk '/\.WarningAlarmHigh/ {print $4}')
WarningAlarmLow=$($voltwar | awk '/\.WarningAlarmLow/ {print $4}')
WarningHigh=$($voltwar | awk '/\.WarningHigh/ {print $4}')
WarningLow=$($voltwar | awk '/\.WarningLow/ {print $4}')
ValueCur=$($voltval | awk '/\.Value/ {print $4}')
ValueMax=$($voltval | awk '/\.MaxValue/ {print $4}')
ValueMin=$($voltval | awk '/\.MinValue/ {print $4}')


result=0
if [ $2 -eq 1 ]; then
if [ "$CriticalAlarmHigh" = "true" ]; then
echo "$1 = $ValueCur - Critical High! (Threshold = $CriticalHigh, Total Range = $ValueMin .. $ValueMax)" >> /etc/rikmail/body.txt
result=1
elif [ "$CriticalAlarmLow" = "true" ]; then
echo "$1 = $ValueCur - Critical Low! (Threshold = $CriticalLow, Total Range = $ValueMin .. $ValueMax)" >> /etc/rikmail/body.txt
result=1
else
result=0
fi
elif [ $2 -eq 2 ]; then
if [ "$CriticalAlarmHigh" = "true" ]; then
echo "$1 = $ValueCur - Critical High! (Threshold = $CriticalHigh, Total Range = $ValueMin .. $ValueMax)" >> /etc/rikmail/body.txt
result=1
elif [ "$CriticalAlarmLow" = "true" ]; then
echo "$1 = $ValueCur - Critical Low! (Threshold = $CriticalLow, Total Range = $ValueMin .. $ValueMax)" >> /etc/rikmail/body.txt
result=1
elif [ "$WarningAlarmHigh" = "true" ]; then
echo "$1 = $ValueCur - Warning High! (Threshold = $WarningHigh, Total Range = $ValueMin .. $ValueMax)" >> /etc/rikmail/body.txt
result=1
elif [ "$WarningAlarmLow" = "true" ]; then
echo "$1 = $ValueCur - Warning Low! (Threshold = $WarningLow, Total Range = $ValueMin .. $ValueMax)" >> /etc/rikmail/body.txt
result=1
else
result=0
fi
else
if [ "$CriticalAlarmHigh" = "true" ]; then
echo "$1 = $ValueCur - Critical High! (Threshold = $CriticalHigh, Total Range = $ValueMin .. $ValueMax)" >> /etc/rikmail/body.txt
result=1
elif [ "$CriticalAlarmLow" = "true" ]; then
echo "$1 = $ValueCur - Critical Low! (Threshold = $CriticalLow, Total Range = $ValueMin .. $ValueMax)" >> /etc/rikmail/body.txt
result=1
elif [ "$WarningAlarmHigh" = "true" ]; then
echo "$1 = $ValueCur - Warning High! (Threshold = $WarningHigh, Total Range = $ValueMin .. $ValueMax)" >> /etc/rikmail/body.txt
result=1
elif [ "$WarningAlarmLow" = "true" ]; then
echo "$1 = $ValueCur - Warning Low! (Threshold = $WarningLow, Total Range = $ValueMin .. $ValueMax)" >> /etc/rikmail/body.txt
result=1
else
echo "$1 = $ValueCur - OK (Good Range = $WarningLow .. $WarningHigh, Total Range = $ValueMin .. $ValueMax)" >> /etc/rikmail/body.txt
result=1
fi
fi
return $result
}

function WriteBody {
res=0
if [ $1 -eq 3 ]; then
HOSTSTATE=$(busctl --no-pager call xyz.openbmc_project.State.Host /xyz/openbmc_project/state/host0 org.freedesktop.DBus.Properties Get ss xyz.openbmc_project.State.Host CurrentHostState | cut -c47-)
echo "HOST STATE = $HOSTSTATE" >> /etc/rikmail/body.txt
fi
Voltages P105_PCH_AUX $1
res=$(( $res + $? ))
Voltages P12V_AUX $1
res=$(( $res + $? ))
Voltages P1V8_PCH $1
res=$(( $res + $? ))
Voltages P3V3 $1
res=$(( $res + $? ))
Voltages PVCCIN_CPU1 $1
res=$(( $res + $? ))
Voltages PVCCIN_CPU2 $1
res=$(( $res + $? ))
Voltages PVCCIO_CPU1 $1
res=$(( $res + $? ))
Voltages PVCCIO_CPU2 $1
res=$(( $res + $? ))
Voltages PVDQ_ABC_CPU1 $1
res=$(( $res + $? ))
Voltages PVDQ_ABC_CPU2 $1
res=$(( $res + $? ))
Voltages PVDQ_DEF_CPU1 $1
res=$(( $res + $? ))
Voltages PVDQ_DEF_CPU2 $1
res=$(( $res + $? ))
Voltages PVNN_PCH_AUX $1
res=$(( $res + $? ))
return $res
}

#--------------------------main-------------------

params=$(tr '_' ';|' < /etc/rikmail/rikmail.conf) 
arr=(${params//;/ })
mode=${arr[0]}
period=${arr[1]}
recipient=${arr[2]}
dateparam=$(date)

modemes="!"
if [ "$mode" -eq 1 ]; then
modemes="errors"
elif [ "$mode" -eq 2 ]; then
modemes="warnings"
else
modemes="values"
fi

from="From: \"Rikor-Scalable EATX Board\" <Rikor-Scalable@rikor.com>"
to="To: \"Administrator\" <$recipient>"
echo $from  > /etc/rikmail/header.txt
echo $to  >> /etc/rikmail/header.txt

oncal="daily"
period1="daily"
mode1="all"

if [ $period -eq 1 ]; then
oncal="*:0/5"
period1="every 5 minutes"
elif [ $period -eq 2 ]; then
oncal="daily"
period1="daily"
else
oncal="daily"
period1="daily"
fi

res2=0
echo "Hi $recipient," > /etc/rikmail/body.txt
echo "Send Mode: $modemes. Period: $period1" >> /etc/rikmail/body.txt

WriteBody $mode
res2=$?
if [ "$res2" -gt 0 ]; then
echo "Bye" >> /etc/rikmail/body.txt
else
echo "All is OK. Bye" >> /etc/rikmail/body.txt
fi
subj="Subject: EATX Board Parameters at $dateparam: $res2 $modemes"
echo $subj  >> /etc/rikmail/header.txt
echo >> /etc/rikmail/header.txt

cat /etc/rikmail/header.txt /etc/rikmail/body.txt > /etc/rikmail/mail.txt

if [ $recipient != "info@example.com" ] && [ $recipient != "" ]; then
/usr/sbin/sendmail -t < /etc/rikmail/mail.txt
sleep 10
fi

