#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "include/parser.h"
#include "include/embed.h"
#include "include/extract.h"

void openFiles();
void closeFiles();

static UserInput parsedInput;
FILE *carrierBmpFile, *msgFile, *outputFile;
static uint8_t *carrierBmp, *msg, *output;
// TODO: Calculate/read each file size
static uint32_t carrierBmpSize = 0, msgSize = 0;

int main(int argc, char *argv[])
{
    PARSE_RET ret = parseInput(argc, argv, &parsedInput);
    if (ret != PARSED_OK)
    {
        // TODO: Print some error code and exit
        printf("ERROR %d\n", ret);
        return EXIT_FAILURE;
    }

    openFiles(parsedInput, &carrierBmp, &msg);

    if (parsedInput.action == EMBED)
    {
        output = embed(carrierBmp, carrierBmpSize, parsedInput.inputFileName, msgSize,
                       parsedInput, msg, parsedInput.carrierFileName);
        fwrite(output, sizeof(*output), carrierBmpSize, outputFile);
    }
    else
    {
        output = extract(carrierBmp, carrierBmpSize, parsedInput);
        fwrite(output, sizeof(*output), msgSize, outputFile);
    }

    closeFiles();

    return EXIT_SUCCESS;
}

void openFiles()
{
    carrierBmpFile = fopen(parsedInput.carrierFileName, "r+");
    msgFile = fopen(parsedInput.inputFileName, "r");
    outputFile = fopen(parsedInput.outputFileName, "w+");
    carrierBmp = malloc(sizeof(*carrierBmp) * carrierBmpSize);
    msg = malloc(sizeof(*msg) * msgSize);

    fread(carrierBmp, sizeof(*carrierBmp), carrierBmpSize, carrierBmpFile);
    fread(msg, sizeof(*msg), msgSize, msgFile);
}

void closeFiles()
{
    fclose(carrierBmpFile);
    fclose(msgFile);
    fclose(outputFile);
}
