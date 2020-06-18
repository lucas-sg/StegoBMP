#include <stdlib.h>
#include "./include/parser.h"


int main(int argc, char *argv[]) {
    UserInput parsedInput;
    parseInput(argc, argv, &parsedInput);
    return EXIT_SUCCESS;
}