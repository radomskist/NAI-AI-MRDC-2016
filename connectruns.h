#!/usr/expect
spawn ssh -t -Y -C niurobotics@10.42.0.1 "cd /home/niurobotics/nai/;sudo ./NaiBrain"
expect "password:""
send "NAI"
expect "password:"
send "NAI"
"interact"
