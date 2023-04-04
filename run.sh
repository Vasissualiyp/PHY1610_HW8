#!/bin/bash

OUT_FILE='longwaveparams.txt'

# This script cleans and then makes and runs with the set number of cores
make clean
make fullclean
export OMP_NUM_THREADS=$1
make -j 10
./wave2d $OUT_FILE
#gprof --line ./wave2d gmon.out | less
