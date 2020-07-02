#!/bin/bash

function embed () {
  for stego_algorithm in "LSB1" "LSB4" "LSBI"
  do
    LOOPS=1

    printf "\n"
    echo $stego_algorithm 

    for ((i=1; i <= LOOPS; i++))
    do
      /usr/bin/time -f'%E' ./main -embed -in "$1" -p tests/resources/lima.bmp -out sampleLSB1.bmp -steg $stego_algorithm

    done
    sleep 0.5

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