#ifndef VALUES_H
#define VALUES_H

// Values types
typedef enum{
    ARG_NULL,
    ARG_INT,
    ARG_REG,
    ARG_RET,
    ARG_FUN
} argumentType_t;

// Instructions types
typedef enum{
    INST_ARG,
    INST_CALL,
    INST_INT,
    INST_MOV,
    INST_RET,
    INST_RETVAL,
    INST_INC,
    INST_DEC,
    INST_JMP,
    INST_JMPIF,
    INST_EQU,
    INST_NOT,
    INST_LT,
    INST_GT,
    INST_LTE,
    INST_GTE,
    INST_ADD,
    INST_SUB,
    INST_MUL,
    INST_DIV,
    INST_MOD
} instructionType_t;

// Instruction names
static const char *instructionNames[] = {
    [INST_CALL] = "call",
    [INST_INT] = "int",
    [INST_MOV] = "mov",
    [INST_RET] = "ret",
    [INST_RETVAL] = "retval",
    [INST_ARG] = "arg",
    [INST_INC] = "inc",
    [INST_DEC] = "dec",
    [INST_JMP] = "jmp",
    [INST_JMPIF] = "jmpif",
    [INST_EQU] = "equ",
    [INST_NOT] = "not",
    [INST_LT] = "lt",
    [INST_GT] = "gt",
    [INST_LTE] = "lte",
    [INST_GTE] = "gte",
    [INST_ADD] = "add",
    [INST_SUB] = "sub",
    [INST_MUL] = "mul",
    [INST_DIV] = "div",
    [INST_MOD] = "mod"
};

#endif // VALUES_H