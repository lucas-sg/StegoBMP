#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "parser.h"
#include "embed.h"
#include "extract.h"


void openFiles();
void closeFiles();
void getFileExtensionFrom(const char* fileName, char *fileExtension);

static UserInput parsedInput;
FILE *carrierBmpFile, *msgFile, *outputFile;
static uint8_t *carrierBmp, *msg, *output;
// TODO: Calculate/read each file size
static uint32_t carrierBmpSize = 0, msgSize = 0;


int main(int argc, char *argv[])
{
    size_t maxFileExtensionLen = 16;    // TODO: Handle this if the file extension is bigger (should be a rare case)
    size_t maxFileNameLen      = 64;    // TODO: Handle this if the file name is bigger (should be a rare case)
    char *fileExtension        = calloc(maxFileExtensionLen, sizeof(*fileExtension));
    char *fileName             = calloc(maxFileNameLen + maxFileExtensionLen, sizeof(*fileName));

    if (parseInput(argc, argv, &parsedInput) != PARSED_OK)
    {
        // TODO: Print some error code and exit
        return EXIT_FAILURE;
    }

    openFiles(parsedInput, &carrierBmp, &msg);

    if (parsedInput.action == EMBED)
    {
        getFileExtensionFrom(parsedInput.outputFileName, fileExtension);
        output     = embed(carrierBmp, carrierBmpSize, msg, msgSize, fileExtension, parsedInput);
        outputFile = fopen(parsedInput.outputFileName, "w+");
        fwrite(output, sizeof(*output), carrierBmpSize, outputFile);
    }
    else
    {
        output     = extract(carrierBmp, carrierBmpSize, fileExtension, parsedInput);
        strcpy(fileName, parsedInput.outputFileName);
        strcat(fileName, fileExtension);
        outputFile = fopen(fileName, "w+");
        fwrite(output, sizeof(*output), msgSize, outputFile);
    }

    closeFiles();

    return EXIT_SUCCESS;
}

void
openFiles()
{
    carrierBmpFile = fopen(parsedInput.carrierFileName, "r+");
    msgFile        = fopen(parsedInput.inputFileName, "r");
    carrierBmp     = malloc(sizeof(*carrierBmp) * carrierBmpSize);
    msg            = malloc(sizeof(*msg) * msgSize);

    fread(carrierBmp, sizeof(*carrierBmp), carrierBmpSize, carrierBmpFile);
    fread(msg,        sizeof(*msg),        msgSize, msgFile);
}

void
closeFiles()
{
    fclose(carrierBmpFile);
    fclose(msgFile);
    fclose(outputFile);
}

void
getFileExtensionFrom(const char* fileName, char *fileExtension)
{
    int i;

    for (i = 0; fileName[i] != '.'; i++);

    for (int j = 0; i < (int)strlen(fileName); i++, j++)
        fileExtension[0] = fileName[i];
}
