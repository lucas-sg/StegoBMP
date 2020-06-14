#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "include/parser.h"

void initOption(OPTION currOpt, const char *name, int has_arg, int val);
void initLongOptions();
void freeLongOpts();

static struct option *longOpts;
/*
    file: Array filled with the int values that represent each option read.
          It is filled at each call of getopt_long_only()
 */
static int *flags;
static int options = 9;

PARSE_RET
parseInput(int argc, char *argv[], UserInput *params) {
    int opt, optIndex = 0;
    char *shortOpts = "";
    struct option parsedOpt;

    opterr = 0; // Set to 0 to avoid seeing default error messages
    optind = 1;
    flags  = calloc(options, sizeof(int));
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
    freeLongOpts();

    return PARSED_OK;
}

void
initLongOptions() {
    longOpts = malloc(sizeof(struct option) * (options + 1));

    initOption(EMBED,   "embed",   no_argument,       'b');
    initOption(EXTRACT, "extract", no_argument,       'x');
    initOption(INPUT,   "in",      required_argument, 'i');
    initOption(CARRIER, "p",       required_argument, 'f');
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
    for (int i = 0; i < options + 1; ++i) {
        free((char *)(*(longOpts + i)).name);
    }

    free(longOpts);
}

enum EXTENSION_CHECK
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
