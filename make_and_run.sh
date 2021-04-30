#!/bin/bash


g++ -O3 -mavx2 -mfma testVector.cpp -o vec

clang++ -O3 -mavx2 testPattern.cpp -o patt

g++ -O0 testCoo.cpp -o coo

g++ testNaive.cpp -o naive

variants=(vec patt coo naive)

RESULTS_FILE="results.txt"

if [ -f $RESULTS_FILE ]; then 
    rm $RESULTS_FILE
fi

touch $RESULTS_FILE

for variant in ${variants[@]}; do

    echo -n "$variant:" >> $RESULTS_FILE

    for i in {1..5}
    do
        exec_time=$("./${variant}")
        if [ $? -ne 0 ]
        then
            echo "Run time error"
            echo >> $RESULTS_FILE
            break
        else

            if [ $i -eq 5 ]
            then
                echo $exec_time >> $RESULTS_FILE
            else
                echo -n "${exec_time}:" >> $RESULTS_FILE
            fi
        fi
    done
done





