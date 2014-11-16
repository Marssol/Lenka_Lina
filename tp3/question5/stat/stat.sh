#!/bin/bash

./main 10 11 

./mainThread 10 11 2
echo "10"

./main 100 111

./mainThread 100 111 2
echo "100"

./main 1000 1111

./mainThread 1000 1111 2
echo "1000"

./main 10000 11111

./mainThread 10000 11111 2
echo "10000"

./main 100000 111111

./mainThread 100000 111111 2
echo "100000"

./main 1000000 1111111

./mainThread 1000000 1111111 2
echo "1000000"

./main 10000000 11111111

./mainThread 10000000 11111111 2
echo "10000000"
