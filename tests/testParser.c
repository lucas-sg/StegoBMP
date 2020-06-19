#include "./include/testParser.h"
#include "../include/parser.h"
#include <assert.h>
#include <stdio.h>


void testParsedOk();
void testMissingEmbedExtract();
void testMissingParameter();
void testMissingArgument();
void testBothEmbedAndExtract();
void testCheckForBMPExtension();
void testOptionalParamsOk();
void testMissingOptionalParam();


void
testParser() {
    printf("\nRunning parser tests:\n\n");

    testParsedOk();
    testMissingEmbedExtract();
    testMissingParameter();
//    testMissingArgument();    // TODO: Check why this test is broken on MacOS
    testBothEmbedAndExtract();
    testCheckForBMPExtension();
    testOptionalParamsOk();
    testMissingOptionalParam();
}

void
testParsedOk() {
    char *argvEmbed[]   = { "program", "-embed", "-in", "somefilePath", "-p", "somefilePath.bmp", "-out", "somefilePath.bmp", "-steg", "LSB1" };
    char *argvExtract[] = { "program", "-extract", "-in", "somefilePath", "-p", "somefilePath.bmp", "-out", "somefilePath.bmp", "-steg", "LSB1" };
    int argc = 10;
    UserInput *parsedInput = malloc(sizeOfUserInputStruct());

    assert(parseInput(argc, argvEmbed, parsedInput) == PARSED_OK);
    assert(parseInput(argc, argvExtract, parsedInput) == PARSED_OK);
    printf("[PASSED] Parsing embedding and extraction\n\n");
    free(parsedInput);
}

void
testMissingParameter() {
    char *missingInput[]   = { "program", "-embed", "-p", "someFilePath.bmp", "-out", "someFilePath.bmp", "-steg", "LSB1" };
    char *missingOutput[]  = { "program", "-embed", "-in", "someFilePath", "-p", "someFilePath.bmp", "-steg", "LSB1" };
    char *missingCarrier[] = { "program", "-embed", "-in", "someFilePath", "-out", "someFilePath.bmp", "-steg", "LSB1" };
    char *missingSteg[]    = { "program", "-embed", "-in", "someFilePath", "-p", "someFilePath.bmp", "-out", "someFilePath.bmp"};
    int argc = 8;
    UserInput *parsedInput = malloc(sizeOfUserInputStruct());

    assert(parseInput(argc, missingInput, parsedInput)   == MISSING_PARAMETER);
    assert(parseInput(argc, missingOutput, parsedInput)  == MISSING_PARAMETER);
    assert(parseInput(argc, missingCarrier, parsedInput) == MISSING_PARAMETER);
    assert(parseInput(argc, missingSteg, parsedInput)    == MISSING_PARAMETER);
    printf("[PASSED] Will fail when missing mandatory parameters\n\n");
    free(parsedInput);
}

void
testMissingArgument() {
    char *missingInput[]   = { "program", "-embed", "-in", "-p", "someFilePath.bmp", "-out", "someFilePath.bmp", "-steg", "LSB1" };
    char *missingOutput[]  = { "program", "-embed", "-in", "someFilePath", "-p", "someFilePath.bmp", "-out", "-steg", "LSB1" };
    char *missingCarrier[] = { "program", "-embed", "-in", "someFilePath", "-p", "-out", "someFilePath.bmp", "-steg", "LSB1" };
    char *missingSteg[]    = { "program", "-embed", "-in", "someFilePath", "-p", "someFilePath.bmp", "-out", "someFilePath.bmp", "-steg" };
    int argc = 9;
    UserInput *parsedInput = malloc(sizeOfUserInputStruct());

    assert(parseInput(argc, missingInput, parsedInput)   == MISSING_ARGUMENT);
    assert(parseInput(argc, missingOutput, parsedInput)  == MISSING_ARGUMENT);
    assert(parseInput(argc, missingCarrier, parsedInput) == MISSING_ARGUMENT);
    assert(parseInput(argc, missingSteg, parsedInput)    == MISSING_ARGUMENT);
    printf("[PASSED] Will fail when missing arguments to mandatory parameters\n\n");
    free(parsedInput);
}

void
testMissingEmbedExtract() {
    char *missingBothEmbedAndExtract[] = { "program", "-in", "someFilePath", "-p", "someFilePath.bmp", "-out", "someFilePath.bmp", "-steg", "LSB1" };
    int argc = 9;
    UserInput *parsedInput = malloc(sizeOfUserInputStruct());

    assert(parseInput(argc, missingBothEmbedAndExtract, parsedInput) == MISSING_ACTION);
    printf("[PASSED] Will fail when there is no action (embed or extract) present\n\n");
    free(parsedInput);
}

void
testBothEmbedAndExtract() {
    char *tooManyActions[] = { "program", "-embed", "-extract", "someFilePath", "-p", "someFilePath.bmp", "-out", "someFilePath.bmp", "-steg", "LSB1" };
    int argc = 10;
    UserInput *parsedInput = malloc(sizeOfUserInputStruct());

    assert(parseInput(argc, tooManyActions, parsedInput) == EMBED_AND_EXTRACT_ERROR);
    printf("[PASSED] Will fail when both 'embed' and 'extract' are present\n\n");
    free(parsedInput);
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

void
testOptionalParamsOk() {
    char *embedWithOptParams[] = { "program",
                                   "-embed",
                                   "-in", "someFilePath",
                                   "-p", "someFilePath.bmp",
                                   "-out", "someFilePath.bmp",
                                   "-steg", "LSB1",
                                   "-a", "aes128",
                                   "-m", "cbc",
                                   "-pass", "dummyPassword" };
    char *extractWithOptParams[] = { "program",
                                     "-extract",
                                     "-p", "someFilePath.bmp",
                                     "-out", "someFilePath.bmp",
                                     "-steg", "LSB1",
                                     "-a", "aes128",
                                     "-m", "cbc",
                                     "-pass", "dummyPassword" };
    int embedArgs = 16;
    int extractArgs = 14;
    UserInput *parsedInput = malloc(sizeOfUserInputStruct());

    assert(parseInput(embedArgs, embedWithOptParams, parsedInput) == PARSED_OK);
    assert(parseInput(extractArgs, extractWithOptParams, parsedInput) == PARSED_OK);
    printf("[PASSED] Parsing embedding and extraction with optional parameters\n\n");
    free(parsedInput);
}

void
testMissingOptionalParam() {
    char *missingEncAlgo[]   = { "program",
                                 "-embed",
                                 "-in", "somefilePath",
                                 "-p", "somefilePath.bmp",
                                 "-out", "somefilePath.bmp",
                                 "-steg", "LSB1",
                                 "-m", "cbc",
                                 "-pass", "password123"};
    char *missingMode[] = { "program",
                            "-embed",
                            "-in", "somefilePath",
                            "-p", "somefilePath.bmp",
                            "-out", "somefilePath.bmp",
                            "-steg", "LSB1",
                            "-a", "aes128",
                            "-pass", "password123"};
    char *missingPassword[] = { "program",
                                "-embed",
                                "-in", "somefilePath",
                                "-p", "somefilePath.bmp",
                                "-out", "somefilePath.bmp",
                                "-steg", "LSB1",
                                "-a", "aes128",
                                "-m", "cbc"};
    int argc = 14;
    UserInput *parsedInput = malloc(sizeOfUserInputStruct());

    assert(parseInput(argc, missingEncAlgo, parsedInput)  == MISSING_PARAMETER);
    assert(parseInput(argc, missingMode, parsedInput)     == MISSING_PARAMETER);
    assert(parseInput(argc, missingPassword, parsedInput) == MISSING_PARAMETER);
    printf("[PASSED] Will fail when missing one optional parameter\n\n");
    free(parsedInput);
}