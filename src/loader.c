#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "error.h"
#include "loader.h"
#include "config.h"


// Find static size needed by a set of instruction
uint32_t findStaticSize(instruction_t* instructions, uint32_t instructionCount) {

    // Create result
    uint32_t neededSize = 0;

    // Loop through instructions
    for (uint32_t i = 0; i < instructionCount; i++) {
        
        instruction_t inst = instructions[i];
  
        if (inst.arg1.type == ARG_REG) {

            if (neededSize == 0) {
                neededSize = 1;
            }

            if (inst.arg1.value > neededSize) {
                neededSize = inst.arg1.value;

            }
        }

        if (inst.arg2.type == ARG_REG) {

            if (neededSize == 0) {
                neededSize = 1;
            }

            if (inst.arg2.value > neededSize) {
                neededSize = inst.arg2.value;

            }
        }
    
    }

    // Return the result
    return neededSize;

}

// Read a function from a stream
function_t load_function(FILE* file) {
    
    // Create result
    function_t fun;

    // Read name length
    uint32_t nameLength;
    if (fread(&nameLength, sizeof(uint32_t), 1, file) != 1) {
        throwLoadingError("Failed to read the function name length.\n");
    }

    // Read name
    fun.name = malloc(nameLength + 1);
    if (!fun.name) {
        throwLoadingError("Failed to allocate memory for the function name.\n");
    }
    if (fread(fun.name, sizeof(char), nameLength, file) != nameLength) {
        throwLoadingError("Failed to read the function name.\n");
    }
    fun.name[nameLength] = '\0';

    // Read public flag
    if (fread(&(fun.pub), sizeof(bool), 1, file) != 1) {
        throwLoadingError("Failed to read the function public flag.\n");
    }

    // Read argument count
    if (fread(&(fun.argCount), sizeof(uint32_t), 1, file) != 1) {
        throwLoadingError("Failed to read the function argument count.\n");
    }

    // Read instruction count
    if (fread(&(fun.instructionCount), sizeof(uint32_t), 1, file) != 1) {
        throwLoadingError("Failed to read the function instruction count.\n");
    }

    // Allocate memory for the instructions
    fun.instructions = malloc(fun.instructionCount * sizeof(instruction_t));
    if (!fun.instructions) {
        throwLoadingError("Failed to allocate memory for the instructions.\n");
    }

    // Load instructions
    fread(fun.instructions, sizeof(instruction_t), fun.instructionCount, file);

    // Setup static size to 0
    fun.staticSize = -1;
    
    // Return the loaded function
    return fun;

}

// Read a complete file from filename
file_t load_file(const char *filename) {

    // Create result
    file_t result;

    // Open file
    FILE *file = fopen(filename, "rb");
    if (!file) {
        throwLoadingError("Failed to open file: \"%s\"\n", filename);
    }

    // Check if signature exist (defined in config.h -> FB_SIGNATURE)
    size_t signatureLength = strlen(FB_SIGNATURE);
    for (size_t i = 0; i < signatureLength; ++i) {
        if (fgetc(file) != FB_SIGNATURE[i]) {
            throwLoadingError("This file isn't a FlowByte executable file.\n");
        }
    }

    // Read the manifest version
    if (fread(&result.manifestVersion, sizeof(double), 1, file) != 1) {
        throwLoadingError("Failed to read the manifest version.\n");
    }
    if (result.manifestVersion != MANIFEST_VERSION) {
        throwLoadingError("This file was compiled with a different version of FlowByte.\n");
    }

    // Read the entry point
    if (fread(&result.entryPoint, sizeof(uint32_t), 1, file) != 1) {
        throwLoadingError("Failed to read the entry point.\n");
    }

    // Read the function count
    if (fread(&result.functionCount, sizeof(uint32_t), 1, file) != 1) {
        throwLoadingError("Failed to read the function count.\n");
    }

    // Allocate memory for the functions
    result.functions = malloc(result.functionCount * sizeof(function_t));
    if (result.functions == NULL) {
        throwLoadingError("Failed to allocate memory for the functions.\n");
    }

    // Load each function
    for (int i = 0; i < result.functionCount; i++) {
        result.functions[i] = load_function(file);
    }

    // Return the loaded file
    return result;

}