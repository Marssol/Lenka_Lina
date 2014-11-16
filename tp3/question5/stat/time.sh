#!/bin/bash

./main 10 11 

./mainThread 10 11 2
echo ""

./main 100 111

./mainThread 100 111 2
echo ""

./main 1000 1111

./mainThread 1000 1111 2
echo ""

./main 10000 11111

./mainThread 10000 11111 2
echo ""

./main 100000 111111

./mainThread 100000 111111 2
echo ""

./main 1000000 1111111

./mainThread 1000000 1111111 2
echo ""

./main 10000000 11111111

./mainThread 10000000 11111111 2
echo ""
