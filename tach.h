#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define nogc 1

#ifndef nogc
#include <gc.h>
#define tach_malloc GC_MALLOC
#define tach_calloc GC_MALLOC
#define tach_realloc GC_REALLOC
#else
#define tach_malloc(n) malloc(n)
#define tach_realloc realloc
#endif


char *tach_string_copy(char *old_string);
long tach_string_length(char *string);
char *tach_string_printf(const char *format, ...);
char tach_tokenize_peek_pfile(FILE *peek_into);