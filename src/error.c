#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "error.h"

void throwError(const char* name, const char* format, ...){
    va_list args;
    va_start(args, format);
    printf("\033[1;31m%s: ", name); // Start red color
    vprintf(format, args);
    printf("\033[0m"); // Reset color
    va_end(args);
    exit(EXIT_FAILURE);
}

void throwLoadingError(const char* format, ...){
    throwError("Loading Error", format);
}

void throwRuntimeError(const char* format, ...){
    throwError("Runtime Error", format);
}