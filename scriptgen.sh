#!/bin/sh
#arg1 = network, arg2 = path, arg3 = password

#/home/niurobotics/nai/
#spawn ssh -t -Y -C niurobotics@10.42.0.1 "cd /home/niurobotics/nai/;sudo ./NaiBrain"

if [ $# -lt 3 ];	then
	echo "Not enough commands.\n"
	echo "Command #1: SSH connection"
	echo "Command #2: Directory to code"
	echo "Command #3: Password"
	return;
fi

#generate connect + run script
echo "#!/usr/expect" > "connectrun.sh"
echo "spawn ssh -t -Y -C $1 \"cd $2;sudo ./NaiBrain\"" >> "connectrun.sh"
echo "expect \"password:\"\"" >> "connectrun.sh"
echo "send \"$3\\\r\"" >> "connectrun.sh"
echo "expect \"password:\"" >> "connectrun.sh"
echo "send \"$3\\\r\"" >> "connectrun.sh"
echo "\"interact\"" >> "connectrun.sh"


#generate upload and make script
echo "#!/usr/bin/expect" > "pushcompile.sh"
echo "spawn scp -r ./src/* $1:$2/src" >> "pushcompile.sh"
echo "expect \"password\"" >> "pushcompile.sh"
echo "send \"$3\\\r\"" >> "pushcompile.sh"
echo "interact" >> "pushcompile.sh"
echo "spawn ssh niurobotics@10.42.0.1 \"cd /home/niurobotics/nai/; make clean; make\"" >> "pushcompile.sh"
echo "expect \"password\"" >> "pushcompile.sh"
echo "send \"NAI\\\r\"" >> "pushcompile.sh"
echo "interact" >> "pushcompile.sh"
echo "\$SHELL" >> "pushcompile.sh"
