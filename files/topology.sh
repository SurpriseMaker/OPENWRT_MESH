#!/bin/sh

###Author:Mr Tsao Bo
###Date: 2020-12-19
###Description: This shell is for topology query.
###		We use telnet to connect to local service, and build a pipe to input sub commands.

ip="127.0.0.1"
port=7777

inputfile=in 
input1="td s2"
input2="q"

rm -fr $inputfile    
mknod $inputfile p  

exec 8<>$inputfile 

telnet $ip $port <&8 &
sleep 1; echo $input1 >> $inputfile   
echo $input2 >> $inputfile 


