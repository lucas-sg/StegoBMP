#include "./include/testParser.h"
#include "../include/parser.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>


void testParsedOk();
void testMissingEmbedExtract();
void testMissingParameter();
void testMissingArgument();
void testBothEmbedAndExtract();
void testCheckForBMPExtension();

size_t MANDATORY_PARAM_SIZE = 9;

void
testParser() {
    printf("\nRunning parser tests:\n\n");

    testParsedOk();
    testMissingEmbedExtract();
    testMissingParameter();
    testMissingArgument();
    testBothEmbedAndExtract();
    testCheckForBMPExtension();
    // TODO: Test optional parameters
}

void
testParseInput() {
}

void
testParsedOk() {
    char *argvEmbed[]   = { "program", "-embed", "-in", "\"somefilePath\"", "-p", "\"somefilePath.bmp\"", "-out", "\"somefilePath.bmp\"", "-steg", "LSB1" };
    char *argvExtract[] = { "program", "-extract", "-in", "\"somefilePath\"", "-p", "\"somefilePath.bmp\"", "-out", "\"somefilePath.bmp\"", "-steg", "LSB1" };
    int argc = 10;
    UserInput *params = malloc(sizeOfUserInputStruct());

    assert(parseInput(argc, argvEmbed, params) == PARSED_OK);
    assert(parseInput(argc, argvExtract, params) == PARSED_OK);
    printf("[PASSED] Parsing embedding and extraction\n\n");
    free(params);
}

void
testMissingParameter() {
    char *missingInput[]   = { "program", "-embed", "-p", "\"somefilePath.bmp\"", "-out", "\"somefilePath.bmp\"", "-steg", "LSB1" };
    char *missingOutput[]  = { "program", "-embed", "-in", "\"somefilePath\"", "-p", "\"somefilePath.bmp\"", "-steg", "LSB1" };
    char *missingCarrier[] = { "program", "-embed", "-in", "\"somefilePath\"", "-out", "\"somefilePath.bmp\"", "-steg", "LSB1" };
    char *missingSteg[]    = { "program", "-embed", "-in", "\"somefilePath\"", "-p", "\"somefilePath.bmp\"", "-out", "\"somefilePath.bmp\""};
    int argc = 8;
    UserInput *params = malloc(sizeOfUserInputStruct());

    assert(parseInput(argc, missingInput, params)   == MISSING_PARAMETER);
    assert(parseInput(argc, missingOutput, params)  == MISSING_PARAMETER);
    assert(parseInput(argc, missingCarrier, params) == MISSING_PARAMETER);
    assert(parseInput(argc, missingSteg, params)    == MISSING_PARAMETER);
    printf("[PASSED] Will fail when missing mandatory parameters\n\n");
    free(params);
}

void
testMissingArgument() {
    char *missingInput[]   = { "program", "-embed", "-in", "-p", "\"somefilePath.bmp\"", "-out", "\"somefilePath.bmp\"", "-steg", "LSB1" };
    char *missingOutput[]  = { "program", "-embed", "-in", "\"somefilePath\"", "-p", "\"somefilePath.bmp\"", "-out", "-steg", "LSB1" };
    char *missingCarrier[] = { "program", "-embed", "-in", "\"somefilePath\"", "-p", "-out", "\"somefilePath.bmp\"", "-steg", "LSB1" };
    char *missingSteg[]    = { "program", "-embed", "-in", "\"somefilePath\"", "-p", "\"somefilePath.bmp\"", "-out", "\"somefilePath.bmp\"", "-steg" };
    int argc = 9;
    UserInput *params = malloc(sizeOfUserInputStruct());

    assert(parseInput(argc, missingInput, params)   == MISSING_ARGUMENT);
    assert(parseInput(argc, missingOutput, params)  == MISSING_ARGUMENT);
    assert(parseInput(argc, missingCarrier, params) == MISSING_ARGUMENT);
    assert(parseInput(argc, missingSteg, params)    == MISSING_ARGUMENT);
    printf("[PASSED] Will fail when missing arguments to mandatory parameters\n\n");
    free(params);
}

void
testMissingEmbedExtract() {
    char *missingBothEmbedAndExtract[] = { "program", "-in", "\"somefilePath\"", "-p", "\"somefilePath.bmp\"", "-out", "\"somefilePath.bmp\"", "-steg", "LSB1" };
    int argc = 9;
    UserInput *params = malloc(sizeOfUserInputStruct());

    assert(parseInput(argc, missingBothEmbedAndExtract, params) == MISSING_ACTION);
    printf("[PASSED] Will fail when there is no action (embed or extract) present\n\n");
    free(params);
}

void
testBothEmbedAndExtract() {
    char *tooManyActions[] = { "program", "-embed", "-extract", "\"somefilePath\"", "-p", "\"somefilePath.bmp\"", "-out", "\"somefilePath.bmp\"", "-steg", "LSB1" };
    int argc = 11;
    UserInput *params = malloc(sizeOfUserInputStruct());

    assert(parseInput(argc, tooManyActions, params) == EMBED_AND_EXTRACT_ERROR);
    printf("[PASSED] Will fail when both 'embed' and 'extract' are present\n\n");
    free(params);
}

void
testCheckForBMPExtension() {
    char *fileNameOk     = "./images/carriers/big_carrier.bmp";
    char *noExtension    = "./images/carriers/big_carrier";
    char *extensionTypo  = "./images/carriers/big_carrier.bmpp";
    char *wrongExtension = "./images/carriers/big_carrier.png";
    char *smallFileName  = ".bmp";

    assert(checkForBMPExtension(fileNameOk)     == BMP_OK);
    assert(checkForBMPExtension(noExtension)    == NOT_BMP_EXTENSION_ERROR);
    assert(checkForBMPExtension(extensionTypo)  == NOT_BMP_EXTENSION_ERROR);
    assert(checkForBMPExtension(wrongExtension) == NOT_BMP_EXTENSION_ERROR);
    assert(checkForBMPExtension(smallFileName)  == SMALL_FILE_NAME_ERROR);
    printf("[PASSED] Will fail when the file name extension isn't .bmp\n\n");
}
