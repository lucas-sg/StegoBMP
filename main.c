#include <stdio.h>
#include <stdlib.h>
#include "./include/parser.h"


int main(int argc, char *argv[]) {
    UserInput userInput;
    parseInput(argc, argv, &userInput);

    printf("%s", userInput.inputFileName);

    return EXIT_SUCCESS;
}