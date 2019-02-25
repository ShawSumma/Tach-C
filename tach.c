#include "tach.h"

char *tach_string_printf(const char *format, ...) {
    va_list vararg_list;
    char *return_string;

    return_string = tach_malloc(sizeof(char) * 80);
    va_start(vararg_list, format);
    long string_length = vsnprintf(return_string, 0, format, vararg_list);
    va_end(vararg_list);

    return_string = tach_malloc(sizeof(char) * (string_length + 1));
    va_start(vararg_list, format);
    vsnprintf(return_string, string_length+1, format, vararg_list);
    va_end(vararg_list);
    return return_string;
}

char tach_tokenize_peek_pfile(FILE *peek_into) {
    char ret = getc(peek_into);
    ungetc(ret, peek_into);
    return ret;
}

long tach_string_length(char *string) {
    long string_index = 0;
    while (string[string_index] != '\0') {
        string_index ++;
    }
    return string_index + 1;
}

char *tach_string_copy(char *old_string) {
    long old_string_size = tach_string_length(old_string);
    char *return_string = tach_malloc(sizeof(char) * (old_string_size + 2));
    long string_index = 0;
    while (string_index <= old_string_size) {
        return_string[string_index] = old_string[string_index];
        string_index++;
    }
    return return_string;
}
