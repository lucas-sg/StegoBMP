#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "parser.h"
#include "embed.h"
#include "extract.h"


void openFiles();

static UserInput parsedInput;
static uint8_t *carrierBmp, *msg, *output;
// TODO: Calculate/read each file size
static uint32_t carrierBmpSize = 0, msgSize = 0;


int main(int argc, char *argv[])
{
    if (parseInput(argc, argv, &parsedInput) != PARSED_OK)
    {
        // TODO: Print some error code and exit
        return EXIT_FAILURE;
    }

    openFiles(parsedInput, &carrierBmp, &msg);

    if (parsedInput.action == EMBED)
        output = embed(carrierBmp, carrierBmpSize, msg, msgSize, parsedInput);
    else
        output = extract(carrierBmp, carrierBmpSize, parsedInput);

    FILE *outputBmpFile = fopen(parsedInput.outputFileName, "w+");
    fwrite(output, sizeof(*output), carrierBmpSize, outputBmpFile);
    fclose(outputBmpFile);

    return EXIT_SUCCESS;
}

void
openFiles()
{
    FILE *carrierBmpFile = fopen(parsedInput.carrierFileName, "r+");
    FILE *msgFile        = fopen(parsedInput.inputFileName, "r");

    carrierBmp    = malloc(sizeof(*carrierBmp) * carrierBmpSize);
    msg           = malloc(sizeof(*msg) * msgSize);

    fread(carrierBmp, sizeof(*carrierBmp), carrierBmpSize, carrierBmpFile);
    fread(msg,        sizeof(*msg),        msgSize, msgFile);
}
