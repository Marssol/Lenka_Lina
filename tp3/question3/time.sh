#!/bin/bash

TIME1=$(date +"%s%N")
./sum 10000000
TIME2=$(date +"%s%N")
TIME3=$(($TIME2-$TIME1))
TIME3=$(($TIME3/1000000))
echo "Execution time in sequential mode : "$TIME3" microseconds"

TIME1=$(date +"%s%N")
./sum_threads 10000000 2
TIME2=$(date +"%s%N")
TIME3=$(($TIME2-$TIME1))
TIME3=$(($TIME3/1000000))
echo "Execution time in multi-_threads (2 _threadss) mode : "$TIME3" microseconds"


TIME1=$(date +"%s%N")
./sum_threads 10000000 10 
TIME2=$(date +"%s%N")
TIME3=$(($TIME2-$TIME1))
TIME3=$(($TIME3/1000000))
echo "Execution time in multi-_threads (4 _threadss) mode : "$TIME3" microseconds"


TIME1=$(date +"%s%N")
./sum_threads 10000000 100
TIME2=$(date +"%s%N")
TIME3=$(($TIME2-$TIME1))
TIME3=$(($TIME3/1000000))
echo "Execution time in multi-_threads (6 _threadss) mode : "$TIME3" microseconds"


TIME1=$(date +"%s%N")
./sum_threads 10000000 1000
TIME2=$(date +"%s%N")
TIME3=$(($TIME2-$TIME1))
TIME3=$(($TIME3/1000000))
echo "Execution time in multi-_threads (8 _threadss) mode : "$TIME3" microseconds"

