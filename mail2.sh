#!/bin/bash

params=$(tr '_' ';|' < /etc/rikmail/rikmail.conf) 
arr=(${params//;/ })
mode=${arr[0]}
period=${arr[1]}
recipient=${arr[2]}


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

sleep 10
