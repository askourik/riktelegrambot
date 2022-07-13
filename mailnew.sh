#!/bin/bash

#period=false
#days=1
#hours=1
#minutes=6
#volt=false
#sens=false
#host=false
#logs=false
#instant=false
#errors=false
#warnings=false
#sending=false
#enable=false
#inmail=xx@xx.xx
#inpass=xx
#smtp=xxx
#port=1234
#outmail=info@example.com
source /etc/rikmail/mailnew.conf
source /etc/rikmail/mailnew.count

logger "mailnew.sh"

if [ $enable == "true" ]; then

mapper="xyz.openbmc_project.ObjectMapper"
allvolt="/xyz/openbmc_project/inventory/system/board/Rikor_Baseboard/all_sensors"
allsens="/xyz/openbmc_project/inventory/system/board/Rikor_CPUs/all_sensors"
allfans="/xyz/openbmc_project/inventory/system/board/Rikor_Fans/all_sensors"
allerr="/xyz/openbmc_project/sensors/critical"
allwarn="/xyz/openbmc_project/sensors/warning"
assoc="xyz.openbmc_project.Association endpoints"

#for var in $period $days $hours $minutes $volt $sens $fans $host $logs $instant $errors $warnings $sending $inmail $inpass $smtp $port $outmail; do
# echo var=$var
#done
#echo ""

#echo "before:$previnstant"

from="From: \"Rikor-Scalable EATX Board\" <$inmail>"
to="To: \"Administrator\" <$outmail>"
dateparam=$(date)
subjperiod="Subject: EATX Board Parameters at $dateparam: Periodic"
subjinstant="Subject: !EATX Board Parameters at $dateparam: Criticals and Warnings"

if [ $period == "true" ] && [ $counter == "0" ]; then
 echo $from > /etc/rikmail/period.txt
 echo $to >> /etc/rikmail/period.txt
 echo $subjperiod >> /etc/rikmail/period.txt
 echo "" >> /etc/rikmail/period.txt
 echo "Hi, $outmail." >> /etc/rikmail/period.txt
 echo "" >> /etc/rikmail/period.txt
 echo "periodic part:" >> /etc/rikmail/period.txt
 if [ $volt == "true" ]; then
  echo "" >> /etc/rikmail/period.txt
  echo "Voltages:" >> /etc/rikmail/period.txt
  echo "" >> /etc/rikmail/period.txt
  voltlist="busctl --system get-property $mapper $allvolt $assoc"
  voltexec=$($voltlist)
  #voltarr=($voltexec)
  IFS=' ' read -a voltarr <<< "$voltexec"
  echo "Number of voltages: ${voltarr[1]}" >> /etc/rikmail/period.txt
  #for i in "${voltarr[@]}"
  for ((i = 2; i < ${#voltarr[@]}; ++i));
  do
   IFS='/"' read -a voltarr2 <<< "${voltarr[$i]}"
   #echo ${voltarr[$i]} ${voltarr2[5]} ${voltarr2[6]}
   if [ ${voltarr2[5]} == "voltage" ]; then
    voltval="busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/${voltarr2[6]}  xyz.openbmc_project.Sensor.Value"
    voltvalcur=$($voltval | awk '/\.Value/ {print $4}')
    echo ${voltarr2[5]} ${voltarr2[6]} $voltvalcur >> /etc/rikmail/period.txt
   elif [ ${voltarr2[5]} == "cfm" ]; then
    voltval="busctl --system introspect xyz.openbmc_project.ExitAirTempSensor /xyz/openbmc_project/sensors/cfm/${voltarr2[6]}  xyz.openbmc_project.Sensor.Value"
    voltvalcur=$($voltval | awk '/\.Value/ {print $4}')
    echo ${voltarr2[5]} ${voltarr2[6]} $voltvalcur >> /etc/rikmail/period.txt
   fi
  done
 fi
 if [ $sens == "true" ]; then
  echo "" >> /etc/rikmail/period.txt
  echo "Sensors:" >> /etc/rikmail/period.txt
  echo "" >> /etc/rikmail/period.txt
  senslist="busctl --system get-property $mapper $allsens $assoc"
  sensexec=$($senslist)
  #sensarr=($sensexec)
  IFS=' ' read -a sensarr <<< "$sensexec"
  echo "Number of sensors: ${sensarr[1]}" >> /etc/rikmail/period.txt
  for ((i = 2; i < ${#sensarr[@]}; ++i));
  do
   IFS='/"' read -a sensarr2 <<< "${sensarr[$i]}"
   #echo ${sensarr[$i]} ${sensarr2[5]} ${sensarr2[6]}
   if [ ${sensarr2[5]} == "temperature" ]; then
    sensval="busctl --system introspect xyz.openbmc_project.CPUSensor /xyz/openbmc_project/sensors/temperature/${sensarr2[6]}  xyz.openbmc_project.Sensor.Value"
    sensvalcur=$($sensval | awk '/\.Value/ {print $4}')
    echo ${sensarr2[5]} ${sensarr2[6]} $sensvalcur >> /etc/rikmail/period.txt
   elif [ ${sensarr2[5]} == "power" ]; then
    sensval="busctl --system introspect xyz.openbmc_project.CPUSensor /xyz/openbmc_project/sensors/power/${sensarr2[6]}  xyz.openbmc_project.Sensor.Value"
    sensvalcur=$($sensval | awk '/\.Value/ {print $4}')
    echo ${sensarr2[5]} ${sensarr2[6]} $sensvalcur >> /etc/rikmail/period.txt
   fi
  done
 fi
 if [ $fans == "true" ]; then
  echo "" >> /etc/rikmail/period.txt
  echo "Fans:" >> /etc/rikmail/period.txt
  echo "" >> /etc/rikmail/period.txt
  fanslist="busctl --system get-property $mapper $allfans $assoc"
  fansexec=$($fanslist)
  #fansarr=($fansexec)
  IFS=' ' read -a fansarr <<< "$fansexec"
  echo "Number of fans: ${fansarr[1]}" >> /etc/rikmail/period.txt
  for ((i = 2; i < ${#fansarr[@]}; ++i));
  do
   IFS='/"' read -a fansarr2 <<< "${fansarr[$i]}"
   #echo ${fansarr[$i]} ${fansarr2[5]} ${fansarr2[6]}
   if [ ${fansarr2[5]} == "temperature" ]; then
    fansval="busctl --system introspect xyz.openbmc_project.HwmonTempSensor /xyz/openbmc_project/sensors/temperature/${fansarr2[6]}  xyz.openbmc_project.Sensor.Value"
    fansvalcur=$($fansval | awk '/\.Value/ {print $4}')
    echo ${fansarr2[5]} ${fansarr2[6]} $fansvalcur >> /etc/rikmail/period.txt
   elif [ ${fansarr2[5]} == "fan_pwm" ]; then
    fansval="busctl --system introspect xyz.openbmc_project.FanSensor /xyz/openbmc_project/sensors/fan_pwm/${fansarr2[6]}  xyz.openbmc_project.Sensor.Value"
    fansvalcur=$($fansval | awk '/\.Value/ {print $4}')
    echo ${fansarr2[5]} ${fansarr2[6]} $fansvalcur >> /etc/rikmail/period.txt
   elif [ ${fansarr2[5]} == "fan_tach" ]; then
    fansval="busctl --system introspect xyz.openbmc_project.FanSensor /xyz/openbmc_project/sensors/fan_tach/${fansarr2[6]}  xyz.openbmc_project.Sensor.Value"
    fansavail="busctl --system introspect xyz.openbmc_project.FanSensor /xyz/openbmc_project/sensors/fan_tach/${fansarr2[6]}  xyz.openbmc_project.State.Decorator.Availability"
    fansfunc="busctl --system introspect xyz.openbmc_project.FanSensor /xyz/openbmc_project/sensors/fan_tach/${fansarr2[6]}  xyz.openbmc_project.State.Decorator.OperationalStatus"
    fansvalcur=$($fansval | awk '/\.Value/ {print $4}')
    fansavailcur=$($fansavail | awk '/\.Available/ {print $4}')
    fansfunccur=$($fansfunc | awk '/\.Functional/ {print $4}')
    if [ $fansavailcur == "true" ] && [ $fansfunccur == "true" ]; then
     echo ${fansarr2[5]} ${fansarr2[6]} $fansvalcur >> /etc/rikmail/period.txt
    else
     echo ${fansarr2[5]} ${fansarr2[6]} Not available or functional >> /etc/rikmail/period.txt
    fi
   fi
  done
 fi
 if [ $host == "true" ]; then
  echo "" >> /etc/rikmail/period.txt
  echo "Host Parameters:" >> /etc/rikmail/period.txt
  echo "" >> /etc/rikmail/period.txt
  echo "Todo" >> /etc/rikmail/period.txt
 fi
 if [ $logs == "true" ]; then
  echo "" >> /etc/rikmail/period.txt
  echo "Logs:" >> /etc/rikmail/period.txt
  echo "" >> /etc/rikmail/period.txt
  cat /var/log/redfish >> /etc/rikmail/period.txt
  echo "" >> /etc/rikmail/period.txt
 fi
 if [ $outmail != "info@example.com" ] && [ $outmail != "" ] && [ $sending == "true" ]; then
  /usr/sbin/sendmail -t < /etc/rikmail/period.txt
  sleep 10
  logger "Periodic Mail sent"
 fi
fi

if [ $instant == "true" ]; then
 echo $from > /etc/rikmail/instant.txt
 echo $to >> /etc/rikmail/instant.txt
 echo $subjinstant >> /etc/rikmail/instant.txt
 echo "" >> /etc/rikmail/instant.txt
 echo "Hi, $outmail." >> /etc/rikmail/instant.txt
 echo "instant part:" >> /etc/rikmail/instant.txt
 if [ $errors == "true" ]; then
  echo "" >> /etc/rikmail/instant.txt
  echo "Criticals:" >> /etc/rikmail/instant.txt
  echo "" >> /etc/rikmail/instant.txt
  errexist="busctl tree $mapper"
  errexistval=$($errexist | grep \-c $allerr)
  if [ $errexistval != "0" ]; then
   errlist="busctl --system get-property $mapper $allerr $assoc"
   errexec=$($errlist)
   IFS=' ' read -a errarr <<< "$errexec"
   echo "Number of errors: ${errarr[1]}" >> /etc/rikmail/instant.txt
   for ((i = 2; i < ${#errarr[@]}; ++i));
   do
    IFS='/"' read -a errarr2 <<< "${errarr[$i]}"
    #echo ${errarr[$i]} ${errarr2[5]} ${errarr2[6]}
    if [ ${errarr2[5]} == "voltage" ]; then
     errval="busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/${errarr2[6]}  xyz.openbmc_project.Sensor.Value"
     errvalcur=$($errval | awk '/\.Value/ {print $4}')
     echo ${errarr2[5]} ${errarr2[6]} $errvalcur >> /etc/rikmail/instant.txt
    #elif [ ${errarr2[5]} == "cfm" ]; then
    # errval="busctl --system introspect xyz.openbmc_project.ExitAirTempSensor /xyz/openbmc_project/sensors/cfm/${errarr2[6]}  xyz.openbmc_project.Sensor.Value"
    # errvalcur=$($errval | awk '/\.Value/ {print $4}')
    # echo ${errarr2[5]} ${errarr2[6]} $errvalcur >> /etc/rikmail/instant.txt
    fi
   done
  fi
 fi
 if [ $warnings == "true" ]; then
  echo "" >> /etc/rikmail/instant.txt
  echo "Warnings:" >> /etc/rikmail/instant.txt
  echo "" >> /etc/rikmail/instant.txt
  warnexist="busctl tree $mapper"
  warnexistval=$($warnexist | grep \-c $allwarn)
  if [ $warnexistval != "0" ]; then
   warnlist="busctl --system get-property $mapper $allwarn $assoc"
   warnexec=$($warnlist)
   IFS=' ' read -a warnarr <<< "$warnexec"
   echo "Number of warnings: ${warnarr[1]}" >> /etc/rikmail/instant.txt
   for ((i = 2; i < ${#warnarr[@]}; ++i));
   do
    IFS='/"' read -a warnarr2 <<< "${warnarr[$i]}"
    #echo ${warnarr[$i]} ${warnarr2[5]} ${warnarr2[6]}
    if [ ${warnarr2[5]} == "voltage" ]; then
     warnval="busctl --system introspect xyz.openbmc_project.ADCSensor /xyz/openbmc_project/sensors/voltage/${warnarr2[6]}  xyz.openbmc_project.Sensor.Value"
     warnvalcur=$($warnval | awk '/\.Value/ {print $4}')
     echo ${warnarr2[5]} ${warnarr2[6]} $warnvalcur >> /etc/rikmail/instant.txt
    #elif [ ${warnarr2[5]} == "cfm" ]; then
    # warnval="busctl --system introspect xyz.openbmc_project.ExitAirTempSensor /xyz/openbmc_project/sensors/cfm/${warnarr2[6]}  xyz.openbmc_project.Sensor.Value"
    # warnvalcur=$($warnval | awk '/\.Value/ {print $4}')
    # echo ${warnarr2[5]} ${warnarr2[6]} $warnvalcur >> /etc/rikmail/instant.txt
    fi
   done
  fi
 fi
 #if [ ${warnarr[1]} != "" ] || [ ${errarr[1]} != "" ]; then
 if [ $warnexistval != "0" ] || [ $errexistval != "0" ]; then
  #if [ $outmail != "info@example.com" ] && [ $outmail != "" ] && [ $sending == "true" ] && [ $previnstant == "false" ]; then
  if [ $outmail != "info@example.com" ] && [ $outmail != "" ] && [ $sending == "true" ] ; then
   if [ $warnings == "true" ] || [ $errors == "true" ]; then
    /usr/sbin/sendmail -t < /etc/rikmail/instant.txt
    sleep 10
    #echo "Instant Mail sent"
   fi
  fi
  previnstant=true
 else
  previnstant=false
 fi
fi
#echo after:$previnstant

logger "counter=$counter"
if [ $counter == "0" ]; then
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
  counter=$divider
fi
counter=$((counter-1))
logger "newcounter=$counter"
echo "counter=$counter" > /etc/rikmail/mailnew.count
echo "previnstant=$previnstant" >> /etc/rikmail/mailnew.count



#if [ $outmail == "info@example.com" ]; then
# echo "not OK"
#fi
#if [ $outmail != "info@example.com" ] && [ $outmail != "" ]; then
# echo "OK"
#fi

else
    logger "mailnew not enabled"
fi


logger "mailnew.sh complete"

