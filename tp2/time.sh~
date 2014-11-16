#!/bin/bash

TIME1=$(date +"%s%N")
./test1 /usr/bin/firefox outfile 
TIME2=$(date +"%s%N")
TIME3=$(($TIME2-$TIME1))
TIME3=$(($TIME3/1000000))
echo "Execution time in sequential mode : "$TIME3" microseconds"

TIME1=$(date +"%s%N")
./test1_bis /usr/bin/firefox outfile
TIME2=$(date +"%s%N")
TIME3=$(($TIME2-$TIME1))
TIME3=$(($TIME3/1000000))
echo "Execution time in multi-thread (2 threads) mode : "$TIME3" microseconds"

