#!/bin/bash

function embed () {
  for stego_algorithm in "LSB1" "LSB4" "LSBI"
  do
    TOTAL_TIME=0
    LOOPS=1

    printf "\n"
    echo $stego_algorithm 

    for ((i=1; i <= LOOPS; i++))
    do
      # START_TIME=$(($(date +%s%N)/1000000))
      $(/usr/bin/time -f'%E' ./main -embed -in $1 -p tests/resources/lima.bmp -out sampleLSB1.bmp -steg $stego_algorithm)
      # END_TIME=$(($(date +%s%N)/1000000))
      # TOTAL_TIME=$((TOTAL_TIME + END_TIME - START_TIME))
    done
    sleep 0.5

    # echo "$stego_algorithm: It took $((TOTAL_TIME / LOOPS)) ms to completely embed the file"
  done
}

printf "Running embedding metrics on three files with different sizes:\n\n"

printf "Embedding loremIpsum.txt (~30KB)\n"
embed "tests/resources/loremIpsum.txt"
printf "\n\n"

sleep 0.5

printf "Embedding sample.bmp (~900KB)\n"
embed "tests/resources/sample.bmp"
printf "\n\n"

printf "Embedding pdfSample.pdf (~2MB)\n"
embed "pdfSample.pdf"