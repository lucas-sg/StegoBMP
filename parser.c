#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "include/parser.h"

void initOption(OPTION currOpt, const char *name, int has_arg, int val);
void initLongOptions();
PARSE_RET checkMissingParams(UserInput *parsedInput);
void freeLongOpts();
void parseAction(int opt, UserInput *parsedInput);
void parseFileName(int opt, UserInput *parsedInput);
void parseStegoAlgo(UserInput *parsedInput);
void parseEncryptionAlgo(UserInput *parsedInput);
void parseEncryptionMode(UserInput *parsedInput);
void parseParamWithArg(int opt, UserInput *parsedInput);

static struct option *longOpts;
/*
    flags: Array filled with the int values that represent each option read.
           It is filled at each call of getopt_long_only()
 */
static int *flags;
static int options = 9;

PARSE_RET
parseInput(int argc, char *argv[], UserInput *parsedInput) {
    int opt, optIndex = 0;
    char *shortOpts = "";
    struct option parsedOpt;

    opterr = 0;     // Set to 0 to avoid seeing default error messages
    optind = 1;
    flags  = calloc(options, sizeof(int));
    initLongOptions();
    parsedInput->status = PARSED_OK;

    while ((opt = getopt_long_only(argc, argv, shortOpts, longOpts,
            &optIndex)) != -1) {
        parsedOpt = longOpts[optIndex];
        opt       = parsedOpt.flag == NULL ? parsedOpt.val : *(parsedOpt.flag);

        switch (opt) {
            case 'b':   // -embed
            case 'x':   // -extract
                parseAction(opt, parsedInput);
                break;
            case 'i':   // -in
            case 'c':   // -p
            case 'o':   // -out
            case 's':   // -steg
            case 'a':   // -a
            case 'm':   // -m
            case 'p':   // -pass
                if (optarg == NULL || optarg[0] == '-')
                    return MISSING_ARGUMENT;
                parseParamWithArg(opt, parsedInput);
                break;
            default:
                if (optarg == NULL || optarg[0] == '-')
                    return MISSING_ARGUMENT;
                break;
        }

        if (parsedInput->status != PARSED_OK)
            return parsedInput->status;
    }

    if (checkMissingParams(parsedInput) != PARSED_OK)
        return parsedInput->status;

    free(flags);
    freeLongOpts();

    return PARSED_OK;
}

void
parseAction(int opt, UserInput *parsedInput) {
    if (opt == 'b') {
        if (flags[EXTR] != 0) {
            parsedInput->status = EMBED_AND_EXTRACT_ERROR;
            return;
        }
        parsedInput->action = EMBED;
    } else if (opt == 'x') {
        if (flags[EMBD] != 0) {
            parsedInput->status = EMBED_AND_EXTRACT_ERROR;
            return;
        }
        parsedInput->action = EXTRACT;
    }
}

void
parseMandatoryParam(int opt, UserInput *parsedInput) {
    switch (opt) {
        case 'c':
        case 'o':
            if (checkForBMPExtension(optarg) != BMP_OK) {
                parsedInput->status = NOT_BMP_FILE_ERROR;
                return;
            }
        case 'i':
            parseFileName(opt, parsedInput);
            break;
        case 's':
            parseStegoAlgo(parsedInput);
            break;
        default:
            break;
    }
}

void
parseOptionalParam(int opt, UserInput *parsedInput) {
    switch (opt) {
        case 'a':
            parseEncryptionAlgo(parsedInput);
            break;
        case 'm':
            parseEncryptionMode(parsedInput);
            break;
        case 'p':
            parsedInput->password = calloc(strlen(optarg) + 1, sizeof(char));
            strcpy(parsedInput->password, optarg);
            break;
        default:
            break;
    }
}

void
parseParamWithArg(int opt, UserInput *parsedInput) {
    if (opt == 'i' || opt == 'c' || opt == 'o' || opt == 's') {
        parseMandatoryParam(opt, parsedInput);
    } else {
        parseOptionalParam(opt, parsedInput);
    }
}

void
parseInputFileName(UserInput *parsedInput) {
    parsedInput->inputFileName = calloc(strlen(optarg) + 1, sizeof(char));
    strcpy(parsedInput->inputFileName, optarg);
}

void
parseCarrierFileName(UserInput *parsedInput) {
    parsedInput->carrierFileName = calloc(strlen(optarg) + 1, sizeof(char));
    strcpy(parsedInput->carrierFileName, optarg);
}

void
parseOutputFileName(UserInput *parsedInput) {
    parsedInput->outputFileName = calloc(strlen(optarg) + 1, sizeof(char));
    strcpy(parsedInput->outputFileName, optarg);
}

void
parseFileName(int opt, UserInput *parsedInput) {
    switch (opt) {
        case 'i':
            parseInputFileName(parsedInput);
            break;
        case 'c':
            parseCarrierFileName(parsedInput);
            break;
        case 'o':
            parseOutputFileName(parsedInput);
            break;
        default:
            break;
    }
}

void
parseStegoAlgo(UserInput *parsedInput) {
    if (strcmp("LSB1", optarg) == 0)
        parsedInput->stegoAlgorithm = LSB1;
    else if (strcmp("LSB4", optarg) == 0)
        parsedInput->stegoAlgorithm = LSB4;
    else if (strcmp("LSBI", optarg) == 0)
        parsedInput->stegoAlgorithm = LSBI;
    else
        parsedInput->status = NOT_STEG_OPTION_ERROR;
}

void
parseEncryptionAlgo(UserInput *parsedInput) {
    if (strcmp("aes128", optarg) == 0)
        parsedInput->encryption = AES_128;
    else if (strcmp("aes192", optarg) == 0)
        parsedInput->encryption = AES_192;
    else if (strcmp("aes256", optarg) == 0)
        parsedInput->encryption = AES_256;
    else if (strcmp("des", optarg) == 0)
        parsedInput->encryption = DES;
    else
        parsedInput->status = NOT_ENC_OPTION_ERROR;
}

void
parseEncryptionMode(UserInput *parsedInput) {
    if (strcmp("ecb", optarg) == 0)
        parsedInput->mode = ECB;
    else if (strcmp("cfb", optarg) == 0)
        parsedInput->mode = CFB;
    else if (strcmp("ofb", optarg) == 0)
        parsedInput->mode = OFB;
    else if (strcmp("cbc", optarg) == 0)
        parsedInput->mode = CBC;
    else
        parsedInput->status = NOT_MODE_OPTION_ERROR;
}

int
missingMandatoryParam() {
    int missingInput = flags[EMBD] != 0 && flags[INPUT] == 0;

    return missingInput || flags[OUTPUT] == 0 || flags[CARRIER] == 0 || flags[STEG] == 0;
}

int
missingOptionalParam() {
    int onlyAlgo = flags[ALGO] != 0 && (flags[MODE] == 0 || flags[PASS] == 0);
    int onlyMode = flags[MODE] != 0 && (flags[ALGO] == 0 || flags[PASS] == 0);
    int onlyPass = flags[PASS] != 0 && (flags[ALGO] == 0 || flags[MODE] == 0);

    return onlyAlgo || onlyMode || onlyPass;
}

PARSE_RET
checkMissingParams(UserInput *parsedInput) {
    if (flags[EMBD] == 0 && flags[EXTR] == 0)
        parsedInput->status = MISSING_ACTION;

    else if (missingMandatoryParam() || missingOptionalParam())
        parsedInput->status = MISSING_PARAMETER;

    return parsedInput->status;
}

void
initLongOptions() {
    longOpts = malloc(sizeof(struct option) * (options + 1));

    initOption(EMBD,    "embed",   no_argument,       'b');
    initOption(EXTR,    "extract", no_argument,       'x');
    initOption(INPUT,   "in",      required_argument, 'i');
    initOption(CARRIER, "p",       required_argument, 'c');
    initOption(OUTPUT,  "out",     required_argument, 'o');
    initOption(STEG,    "steg",    required_argument, 's');
    initOption(ALGO,    "a",       required_argument, 'a');
    initOption(MODE,    "m",       required_argument, 'm');
    initOption(PASS,    "pass",    required_argument, 'p');
    initOption(END,     NULL,      0,           0);
}

void
initOption(OPTION currOpt, const char *name, int has_arg, int val) {
    struct option *longOpt = longOpts + currOpt;

    if (currOpt == END) {
        longOpt->name = name;
        longOpt->flag = NULL;
    } else {
        longOpt->name = malloc(sizeof(char) * (strlen(name) + 1));
        strcpy((char *)longOpt->name, name);
        // flag is the direction of where the val value should be saved when read
        longOpt->flag = flags + currOpt;
    }

    longOpt->has_arg = has_arg;
    longOpt->val     = val;
}

void
freeLongOpts() {
    for (int i = 0; i < options + 1; ++i)
        free((char *)(*(longOpts + i)).name);

    free(longOpts);
}

EXTENSION_CHECK
checkForBMPExtension(const char *fileName) {
    const char *bmp = ".bmp";

    if (strlen(fileName) <= strlen(bmp))
        return SMALL_FILE_NAME_ERROR;

    if (strcmp(fileName + strlen(fileName) - strlen(bmp), bmp) == 0)
        return BMP_OK;

    return NOT_BMP_EXTENSION_ERROR;
}

size_t
sizeOfUserInputStruct() {
    return sizeof(UserInput);
}
