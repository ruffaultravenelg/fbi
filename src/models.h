#ifndef MODELS_H
#define MODELS_H

#include <stdint.h>
#include <stdbool.h>
#include "const.h"

typedef struct{
    argumentType_t type;
    int32_t value;
} argument_t;
void printArgument(argument_t arg);

typedef struct {
    instructionType_t type;
    argument_t arg1;
    argument_t arg2;
} instruction_t;
void printInstruction(instruction_t inst);

typedef struct {
    char* name;
    bool pub;
    uint32_t argCount;
    int32_t staticSize;
    uint32_t instructionCount;
    instruction_t* instructions;
} function_t;
void printFunction(function_t fun);

typedef struct {
    double manifestVersion;
    uint32_t entryPoint;
    uint32_t functionCount;
    function_t* functions;
} file_t;
void printFile(file_t file);

#endif // MODELS_H