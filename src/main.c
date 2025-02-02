#include <stdio.h>
#include <string.h>
#include "config.h"
#include "loader.h"
#include "interpreter.h"
#include "models.h"

// Function to display the version of the program
void displayVersion() {
    printf("FlowByte Interpreter Version: %s\n", PROGRAM_VERSION);
}

// Function to display help information
void displayHelp() {
    printf("FlowByte Interpreter\n");
    printf("Usage: fb [options] <file>\n");
    printf("Options:\n");
    printf("  -h, --help     Display this information\n");
    printf("  -v, --version  Display version information\n");
    printf("  -d, --debug    Enable debug mode\n");
    printf("  -r, --read     Read the file\n");
}

// Function to check if a given argument matches a short or long flag
int checkFlag(const char *arg, const char *shortFlag, const char *longFlag) {
    return (strcmp(arg, shortFlag) == 0 || strcmp(arg, longFlag) == 0);
}

int main(int argc, char const *argv[]) {
    int flagHelp = 0;               // Flag to indicate if help should be displayed
    int flagVersion = 0;            // Flag to indicate if version should be displayed
    int flagDebug = 0;              // Flag to indicate if debug mode should be enabled
    int flagRead = 0;               // Flag to indicate if the file should be read
    const char *filepath = NULL;    // Variable to store the input file path

    // Loop through command line arguments
    for (int i = 1; i < argc; ++i) {
        if (checkFlag(argv[i], "-h", "--help")) {
            flagHelp = 1;
        } else if (checkFlag(argv[i], "-v", "--version")) {
            flagVersion = 1;
        } else if (checkFlag(argv[i], "-d", "--debug")) {
            flagDebug = 1;
        } else if (checkFlag(argv[i], "-r", "--read")) {
            flagRead = 1;
        } else if (filepath == NULL) {
            filepath = argv[i];
        }
    }

    // Display help if the help flag is set
    if (flagHelp) {
        displayHelp();
        return 0;
    }

    // Display version if the version flag is set
    if (flagVersion) {
        displayVersion();
        return 0;
    }

    // Check if no input file is provided
    if (filepath == NULL) {
        printf("Error: No input file\n");
        return 1;
    }

    // Load file
    file_t file = load_file(filepath);

    // Check if the read flag is set
    if (flagRead) {
        printFile(file);
        return 0;
    }

    // Interpret file
    interpret_file(file);

    // Exit
    return 0;

}
