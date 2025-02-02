#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "interpreter.h"
#include "models.h"
#include "error.h"

// Argument shortcut
#define ARG1 getValue(inst.arg1, &registers, &registerCount)
#define ARG2 getValue(inst.arg2, &registers, &registerCount)
#define SET_REGISTER(reg, value) setRegister(&registers, &registerCount, reg, value)
#define GET_REGISTER(reg) getRegister(&registers, &registerCount, reg)

// Interupts
struct interupt_t {
    void (*execute)(int32_t** registers, int32_t* register_count);
    uint32_t argCount;
};
void INT_putc(int32_t** registers, int32_t* register_count);
void INT_puti(int32_t** registers, int32_t* register_count);
struct interupt_t interupts[] = {
    {INT_putc, 1}, // PUTC
    {INT_puti, 1}, // PUTI
};
#define INTERUPT_COUNT (sizeof(interupts) / sizeof(struct interupt_t))

// Main file
file_t file;

// Registers
int32_t REGISTER_retval = 0;
int32_t ARGS[256];
int32_t ARG_POINTER = 0;

// Reallocate register
void reallocateRegisters(int32_t** registers, int32_t* register_count, int32_t size) {
    *registers = realloc(*registers, size * sizeof(int32_t));
    if (*registers == NULL) {
        throwRuntimeError("Failed to reallocate registers.\n");
    }
    *register_count = size;
}

// Get register
int32_t getRegister(int32_t** registers, int32_t* register_count, int32_t reg) {
    if (reg >= *register_count) {
        reallocateRegisters(registers, register_count, reg + 1);
    }
    return *(*registers + reg);
}

// Set register
void setRegister(int32_t** registers, int32_t* register_count, int32_t reg, int32_t value) {
    if (reg >= *register_count) {
        reallocateRegisters(registers, register_count, reg + 1);
    }
    *(*registers + reg) = value;
}

// Get value
int32_t getValue(argument_t arg, int32_t** registers, int32_t* register_count) {
    switch (arg.type) {

        case ARG_INT:
            return arg.value;

        case ARG_REG:
            return getRegister(registers, register_count, arg.value);

        case ARG_RET:
            return REGISTER_retval;

        case ARG_FUN:
            return arg.value;

        case ARG_NULL:
            throwRuntimeError("Null argument.\n");

        default:
            throwRuntimeError("Unknown argument type.\n");

    }
    return 0;
}

// Retrieve pushed arguments
void retrieveArguments(int32_t** registers, int32_t* register_count, uint32_t argCount) {
    
    // If size isn't enough, reallocate
    if (argCount > (int32_t)*register_count) {
        reallocateRegisters(registers, register_count, argCount);
    }

    // If argCount doesn't match passed arguments
    if (argCount != ARG_POINTER) {
        throwRuntimeError("Argument count mismatch.\n");
    }

    // Set arguments
    for (uint32_t i = 0; i < argCount; i++) {
        setRegister(registers, register_count, i, ARGS[i]);
    }

    // Reset argument pointer
    ARG_POINTER = 0;

}

// Init registers
int32_t* initRegisters(int32_t calculatedRegisterCount) {

    // Create registers
    int32_t* registers = NULL;

    // Allocate static size
    if (calculatedRegisterCount != -1){
        registers = calloc(calculatedRegisterCount, sizeof(int32_t));
        if (registers == NULL) {
            throwRuntimeError("Failed to allocate memory for the registers.\n");
        }
    }

    // Return registers
    return registers;

}

// Execute a interupt
void executeInterupt(uint32_t interupt){

    // Check if the intrupt is valid
    if (interupt >= INTERUPT_COUNT) {
        throwRuntimeError("Interupt does not exist.\n");
    }

    // Create registers
    int32_t registerCount = interupts[interupt].argCount;
    int32_t* registers = initRegisters(registerCount);

    // Retrieve arguments
    retrieveArguments(&registers, &registerCount, registerCount);

    // Execute interupt
    interupts[interupt].execute(&registers, &registerCount);
    
}

// Execute a function
void executeFunction(function_t* fun) {

    // Program counter
    uint32_t pc = 0;

    // Allocate static registers
    int32_t registerCount = fun->staticSize == -1 ? 0 : fun->staticSize;
    int32_t* registers = initRegisters(fun->staticSize);

    // Retrieve arguments
    retrieveArguments(&registers, &registerCount, fun->argCount);

    // Loop through instructions
    while (pc < fun->instructionCount) {

        // Get instruction
        instruction_t inst = fun->instructions[pc];

        // Execute instruction
        switch (inst.type) {
            
            case INST_ARG:
                ARGS[ARG_POINTER] = ARG1;
                ARG_POINTER++;
                if (ARG_POINTER >= 256) {
                    throwRuntimeError("Too many arguments.\n");
                }
                break;

            case INST_CALL:
                executeFunction(&file.functions[ARG1]);
                break;
            
            case INST_INT:
                executeInterupt(ARG1);
                break;
            
            case INST_MOV:
                setRegister(&registers, &registerCount, ARG1, ARG2);
                break;
            
            case INST_RET:
                goto cleanup;
            
            case INST_RETVAL:
                REGISTER_retval = ARG1;
                break;

            case INST_INC:
                SET_REGISTER(ARG1, GET_REGISTER(ARG1) + 1);
                break;

            case INST_DEC:
                SET_REGISTER(ARG1, GET_REGISTER(ARG1) - 1);
                break;

            case INST_JMP:
                pc = ARG1 - 1;
                break;

            case INST_JMPIF:
                if (ARG2 != 0) {
                    pc = ARG1 - 1;
                }
                break;

            case INST_EQU:
                REGISTER_retval = ARG1 == ARG2;
                break;

            case INST_NOT:
                REGISTER_retval = !ARG1;
                break;
            
            default:
                throwRuntimeError("Unknown instruction type (%d).\n", inst.type);
                break;

        }

        // Increment program counter
        pc++;

    }

cleanup:
    // Free registers
    free(registers);

    // Update register count
    if (registerCount > fun->staticSize){
        fun->staticSize = registerCount;
    }
    
}

// Execute a file
void interpret_file(file_t targeted_file){

    // Set file
    file = targeted_file;

    // Check if the entry point exist
    if (file.entryPoint >= file.functionCount) {
        throwLoadingError("Entry point does not exist.\n");
    }

    // Execute the entry point
    executeFunction(&file.functions[file.entryPoint]);

}

// PUTC Interupt
void INT_putc(int32_t** registers, int32_t* register_count){
    printf("%c", getRegister(registers, register_count, 0));
}

// PUTI Interupt
void INT_puti(int32_t** registers, int32_t* register_count){
    printf("%d", getRegister(registers, register_count, 0));
}