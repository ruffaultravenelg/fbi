#include "models.h"
#include <stdio.h>
#include "error.h"

void printArgument(argument_t arg) {
    switch (arg.type) {
        case ARG_NULL:
            break;

        case ARG_INT:
            printf("%d", arg.value);
            break;
        
        case ARG_REG:
            printf("$%d", arg.value);
            break;

        case ARG_RET:
            printf("?ret");
            break;

        case ARG_FUN:
            printf("fun:%d", arg.value);
            break;

        default:
            throwError("Unknown argument type.\n", "%d", arg.type);
            break;
    }
}

void printInstruction(instruction_t inst) {

    if (inst.type < sizeof(instructionNames) / sizeof(instructionNames[0]) && instructionNames[inst.type]) {
        printf("%s", instructionNames[inst.type]);
    } else {
        throwError("Unknown instruction type", "%d", inst.type);
    }

    printf(" ");
    printArgument(inst.arg1);
    printf(", ");
    printArgument(inst.arg2);
}

void printFunction(function_t fun) {
    if (fun.pub) {
        printf("pub ");
    }
    printf("%s %d\n", fun.name, fun.argCount);
    for (uint32_t i = 0; i < fun.instructionCount; i++) {
        printf("\t");
        printInstruction(fun.instructions[i]);
        printf("\n");
    }
}

void printFile(file_t file) {
    printf("-------------------\n");
    printf("Manifest Version: %.2f\n", file.manifestVersion);
    printf("Entry Point: %u\n", file.entryPoint);
    printf("Function Count: %u\n", file.functionCount);
    printf("-------------------\n");
    for (uint32_t i = 0; i < file.functionCount; i++) {
        printFunction(file.functions[i]);
    }
    printf("-------------------\n");
}