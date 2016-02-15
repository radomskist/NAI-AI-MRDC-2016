#Copies source files onto laptop and compiles
scp -r ./src/* niurobotics@10.42.0.1:/home/niurobotics/nai/src
ssh niurobotics@10.42.0.1 "cd /home/niurobotics/nai/; make clean; make"
$SHELL
