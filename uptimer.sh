params=$(tr '_' ';|' < /etc/rikmail/rikmail.conf) 
arr=(${params//;/ })
mode=${arr[0]}
period=${arr[1]}
recipient=${arr[2]}
oncal="*:0/5"

if [ $period -eq 1 ]; then
oncal="*:0/5"
elif [ $period -eq 2 ]; then
oncal="daily"
fi

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

logger "uptime.sh complete"
