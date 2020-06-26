#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "include/extract.h"
#include "include/parser.h"
#include "include/embed.h"

void openFiles();
void closeFiles();
void getFileExtensionFrom(const char *fileName, char *fileExtension);

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
        printf("%s\n", parsedInput.carrierFileName);
        if ((carrierBmp = parseBmp(parsedInput.carrierFileName)) == NULL)
            return EXIT_FAILURE;
        printf("ANTES DE EMBEBER\n");
        embed(parsedInput, carrierBmp, msg);
        printf("DESPUES DE EMBEBER\n");
        saveBmp(carrierBmp, parsedInput.outputFileName);
        printf("DESPUES DE GUARDAR EL SAVE\n");
    }
    else
    {
        if ((carrierBmp = parseBmp(parsedInput.inputFileName)) == NULL)
            return EXIT_FAILURE;

        msg = malloc(sizeof(MESSAGE));

        if (extract(carrierBmp, msg, parsedInput) != EXTRACTION_SUCCEEDED)
            return EXIT_FAILURE;

        char fileName[64];
        printf("CERRAMOS\n");
        saveMessage(msg, parsedInput.outputFileName);
    }

    // closeFiles();

    return EXIT_SUCCESS;
}

void closeFiles()
{
    fclose(carrierBmpFile);
    fclose(msgFile);
    fclose(outputFile);
}
