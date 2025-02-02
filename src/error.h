#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdarg.h>

void throwLoadingError(const char* format, ...);
void throwRuntimeError(const char* format, ...);
void throwError(const char* name, const char* format, ...);

#endif // ERROR_H