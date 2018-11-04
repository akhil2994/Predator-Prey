#!/bin/bash

cd "$(dirname "$0")"
#load defualt parameters from file
source params.cfg

#create output folder if it doesn't exist
if [ ! -d "output" ]
then
    	mkdir output
fi

#create bin folder if it doesn't exist
if [ ! -d "bin" ]
then
    	mkdir bin
fi

#create processed folder if it doesn't exist
if [ ! -d "processed" ]
then
    	mkdir processed
fi

#create obj folder if it doesn't exist
if [ ! -d "obj" ]
then
    	mkdir obj
fi

# run tests and compile project 
make test_results.txt || printf "****\n****\n****\nCould not run tests. Make sure that you have CUnit installed\n****\n****\n****\n" 
make &> /dev/null

# HARE_BIRTH_RATE=0.08
# PRAY_CONSUMPTION_RATE=0.04
# HARE_DIFUSSION_RATE=0.2
# PUMA_BIRTH_RATE=0.02
# PUMA_MORTALITY_RATE=0.06
# PUMA_DIFUSSION_RATE=0.2
# TIMESTEP=0.4

#get user-defined modifications to parameters
while getopts ":r:a:b:m:k:l:t:p:" opt; do
  case $opt in
    r)
      HARE_BIRTH_RATE=$OPTARG
      echo "HARE_BIRTH_RATE set to $HARE_BIRTH_RATE"
      ;;
     a)
      PRAY_CONSUMPTION_RATE=$OPTARG
      echo "PRAY_CONSUMPTION_RATE set to $PRAY_CONSUMPTION_RATE"
      ;;
     b)
      PUMA_BIRTH_RATE=$OPTARG
      echo "PUMA_BIRTH_RATE set to $PUMA_BIRTH_RATE"
      ;;
     m)
      PUMA_MORTALITY_RATE=$OPTARG
      echo "PUMA_MORTALITY_RATE set to $PUMA_MORTALITY_RATE"
      ;;
     k)
      HARE_DIFUSSION_RATE=$OPTARG
      echo "HARE_DIFUSSION_RATE set to $HARE_DIFUSSION_RATE"
      ;;
     l)
      PUMA_DIFUSSION_RATE=$OPTARG
      echo "PUMA_DIFUSSION_RATE set to $PUMA_DIFUSSION_RATE"
      ;;
     t)
      TIMESTEP=$OPTARG
      echo "TIMESTEP set to $TIMESTEP"
      ;;
     p)
      PRINT_STEP=$OPTARG
      echo "PRINT_STEP set to $PRINT_STEP"
      ;;
  esac
done

list="$(ls input/)"


if [ "$(ls -A input/)" ]
then
  for INPUT in $list
  do

    echo "processing $INPUT..."
    if [ ! -d "output/$INPUT" ]
    then
          mkdir output/$INPUT
    fi

    if [ ! -d "output/$INPUT/hare" ]
    then
          mkdir output/$INPUT/hare
    fi

    if [ ! -d "output/$INPUT/puma" ]
    then
          mkdir output/$INPUT/puma
    fi

    ./bin/run_simulation input/$INPUT $HARE_BIRTH_RATE $PRAY_CONSUMPTION_RATE $HARE_DIFUSSION_RATE $PUMA_BIRTH_RATE $PUMA_MORTALITY_RATE $PUMA_DIFUSSION_RATE $TIMESTEP $PRINT_STEP output/$INPUT

    
    mv input/$INPUT processed/
  done

else
  echo "no files to process in input folder. Stopping."
fi

