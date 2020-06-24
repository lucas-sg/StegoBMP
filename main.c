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

int main(int argc, char *argv[])
{
    MESSAGE *msg;
    BMP *carrierBmp;
    OUTPUT_BMP *output;
    PARSE_RET ret = parseInput(argc, argv, &parsedInput);

    if (ret != PARSED_OK)
    {
        // TODO: Print some error code and exit
        printf("ERROR %d\n", ret);
        return EXIT_FAILURE;
    }

    if (parsedInput.action == EMBED)
    {
        if ((msg = parseMessage(parsedInput.inputFileName)) == NULL)
            return EXIT_FAILURE;

        if ((carrierBmp = parseBmp(parsedInput.carrierFileName)) == NULL)
            return EXIT_FAILURE;

        if ((output = embed(parsedInput, carrierBmp, msg)) == NULL)
            return EXIT_FAILURE;

        fwrite(output->data, sizeof(uint8_t), output->size, outputFile);
    }
    else
    {
        output = extract(parsedInput.inputFileName, carrierBmpSize, parsedInput);
        if ((output = extract(parsedInput, carrierBmp, msg)) == NULL)
            return EXIT_FAILURE;

        fwrite(output->data, sizeof(uint8_t), output->size, outputFile);
    }

     closeFiles();

    return EXIT_SUCCESS;
}

//void openFiles()
//{
//    carrierBmpFile = fopen(parsedInput.carrierFileName, "r+");
//    msgFile = fopen(parsedInput.inputFileName, "r");
//    outputFile = fopen(parsedInput.outputFileName, "w+");
//    carrierBmp = malloc(sizeof(*carrierBmp) * carrierBmpSize);
//    msg = malloc(sizeof(*msg) * msgSize);
//
//    fread(carrierBmp, sizeof(*carrierBmp), carrierBmpSize, carrierBmpFile);
//    fread(msg, sizeof(*msg), 102, msgFile); // FIX THIS
//}

void closeFiles()
{
    fclose(carrierBmpFile);
    fclose(msgFile);
    fclose(outputFile);
}
