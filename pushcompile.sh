#!/usr/bin/expect
spawn scp -r ./src/* niurobotics@10.42.0.1:/home/niurobotics/nai//src
expect "password"
send "NAI\r"
interact
spawn ssh niurobotics@10.42.0.1 "cd /home/niurobotics/nai/; make clean; make"
expect "password"
send "NAI\r"
interact
$SHELL
