#!/bin/bash
/usr/sbin/postfix start
sleep 10
/usr/sbin/sendmail -t < /etc/rikmail/mail$1.txt
sleep 10
/usr/sbin/postfix stop
sleep 10

