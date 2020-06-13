#include "./include/testParser.h"
#include "../include/parser.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>


void testParsedOk();
void testMissingEmbedExtract();
void testMissingParameter();
void testMissingArgument();
void testCheckForBMPExtension();

size_t MANDATORY_PARAM_SIZE = 9;

void
testParser() {
    printf("\nRunning parser tests:\n\n");

    testParsedOk();
    testMissingEmbedExtract();
    testMissingParameter();
    // testMissingArgument();
    testCheckForBMPExtension();
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

    printf("Opcion %d\n", parseInput(argc, argvEmbed, params));
    assert(parseInput(argc, argvEmbed, params) == PARSED_OK);
    printf("Opcion %d\n", parseInput(argc, argvExtract, params));
    assert(parseInput(argc, argvExtract, params) == PARSED_OK);
    printf("[PASSED] Parsing embedding and extraction\n\n");
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
}

void
testMissingEmbedExtract() {
    char *argv[] = { "program", "-in", "\"somefilePath\"", "-p", "\"somefilePath.bmp\"", "-out", "\"somefilePath.bmp\"", "-steg", "LSB1" };
    int argc = 9;
    UserInput *params = malloc(sizeOfUserInputStruct());

    assert(parseInput(argc, argv, params) == MISSING_ACTION);
    printf("[PASSED] Will fail when there is no action (embed or extract) present\n\n");    
}

void
testBothEmbedAndExtract() {
    char *argv[] = { "program", "-embed", "-extract", "\"somefilePath\"", "-p", "\"somefilePath.bmp\"", "-out", "\"somefilePath.bmp\"", "-steg", "LSB1" };
    int argc = 11;
    UserInput *params = malloc(sizeOfUserInputStruct());

    assert(parseInput(argc, argv, params) == EMBED_AND_EXTRACT_ERROR); 
}

void
testCheckForBMPExtension() {
    char *okFileName     = "./images/carriers/big_carrier.bmp";
    char *noExtension    = "./images/carriers/big_carrier";
    char *extensionTypo  = "./images/carriers/big_carrier.bmpp";
    char *wrongExtension = "./images/carriers/big_carrier.png";

    // assert(checkForBMPExtension(okFileName)     == BMP_OK);
    // assert(checkForBMPExtension(noExtension)    == NOT_BMP_EXTENSION_ERROR);
    // assert(checkForBMPExtension(extensionTypo)  == NOT_BMP_EXTENSION_ERROR);
    // assert(checkForBMPExtension(wrongExtension) == NOT_BMP_EXTENSION_ERROR);
}
