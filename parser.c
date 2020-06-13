#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "include/parser.h"

void printUsage();
void initOption(OPTION currOpt, const char *name, int has_arg, int val);
struct option * initLongOptions();

static struct option *longOpts;
static int *flags;

PARSE_RET
parseInput(int argc, char *argv[], UserInput *params) {
    int opt = 0, optIndex = 0, options = 6;
    char *shortOpts = "";
    struct option parsedOpt  = {};

    opterr = 0; // Set to 0 to avoid seeing default error messages
    optind = 1;
    flags  = malloc(sizeof(int) * options);
    initLongOptions();

    while ((opt = getopt_long_only(argc, argv, shortOpts, longOpts, 
            &optIndex)) != -1) {
        parsedOpt = longOpts[optIndex];
        opt       = parsedOpt.flag == NULL ? parsedOpt.val : *(parsedOpt.flag);

        switch (opt) {
            case 'b':
                if (flags[EXTRACT] != 0)
                    return EMBED_AND_EXTRACT_ERROR;
                break;

            case 'x':
                if (flags[EMBED] != 0)
                    return EMBED_AND_EXTRACT_ERROR;
                break;
    
            case '?':
                return MISSING_ARGUMENT;

            case ':':
                return MISSING_ARGUMENT;

            default:
                if (optarg == NULL || optarg[0] == '-')
                    return MISSING_ARGUMENT;
                break;
        }
    }

    if (flags[EMBED] == 0 && flags[EXTRACT] == 0)
        return MISSING_ACTION;

    if ((flags[EMBED] != 0 && flags[INPUT] == 0) || 
        flags[OUTPUT] == 0 || flags[CARRIER] == 0 || flags[STEG] == 0)
        return MISSING_PARAMETER;
    
    free(flags);
    free(longOpts);

    return PARSED_OK;
}

int
handleInput(int input) {
    return 1;
}

enum EXTENSION_CHECK
checkForBMPExtension(const char *fileName) {
    char *bmp = ".bmp";
    size_t len = strlen(fileName);

    return -1;
}

struct option *
initLongOptions() {
    int options = 9;
    longOpts = malloc(sizeof(struct option) * (options + 1));
    
    initOption(EMBED,   "embed",   no_argument,       'b');
    initOption(EXTRACT, "extract", no_argument,       'x');
    initOption(INPUT,   "in",      required_argument, 'i');
    initOption(CARRIER, "p",       required_argument, 'p');
    initOption(OUTPUT,  "out",     required_argument, 'o');
    initOption(STEG,    "steg",    required_argument, 's');
    // initOption(ALGO,    "a",       required_argument, 'a');
    // initOption(MODE,    "m",       required_argument, 'm');
    // initOption(PASS,    "pass",    required_argument, 'w');
    initOption(END,     NULL,      0,                  0);

    return longOpts;
}

void
initOption(OPTION currOpt, const char *name, int has_arg, int val) {
    struct option *longOpt = longOpts + currOpt;
    
    if (currOpt == END) {
        longOpt->name = name;
        longOpt->flag = NULL;
    } else {
        longOpt->name = malloc(sizeof(char) * (strlen(name) + 1));
        strcpy(longOpt->name, name);
        longOpt->flag = flags + currOpt;
    }

    longOpt->has_arg = has_arg;
    longOpt->val     = val;
}

size_t
sizeOfUserInputStruct() {
    return sizeof(UserInput);
}
