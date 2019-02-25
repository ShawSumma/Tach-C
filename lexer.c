#include "lexer.h"

struct tach_tokenize_token_group tach_tokenize_file_until(FILE *input_file, char end_of_tokens) {
    struct tach_tokenize_token_group return_group;
    return_group.memory_count_tokens = 40;
    return_group.count_tokens = 0;
    return_group.token_index = 0;
    return_group.tokens = tach_malloc(sizeof(struct tach_tokenize_token)*return_group.memory_count_tokens);
    while (1) {
        if (return_group.count_tokens >= return_group.memory_count_tokens) {
            return_group.memory_count_tokens *= 1.5;
            return_group.tokens = tach_realloc(return_group.tokens, sizeof(struct tach_tokenize_token)*return_group.memory_count_tokens);
        }
        struct tach_tokenize_token current_token = tach_tokenize_read_token(input_file);
        return_group.tokens[return_group.count_tokens] = current_token;
        return_group.count_tokens ++;
        if (tach_tokenize_peek_pfile(input_file) == end_of_tokens) {
            break;
        }
        if (current_token.token_kind == TACH_TOKENIZE_TOKEN_KIND_END_OF_FILE) {
            break;
        }
    }
    return return_group;
}

struct tach_tokenize_token_group tach_tokenize_file_whole(FILE *input_file) {
    return tach_tokenize_file_until(input_file, EOF);
}

struct tach_tokenize_token tach_tokenize_create_token_empty(enum tach_tokenize_token_kind token_kind) {
    struct tach_tokenize_token return_token;
    return_token.token_kind = token_kind;
    return_token.token_string = "";
    return return_token;
}

struct tach_tokenize_token tach_tokenize_create_token_string(enum tach_tokenize_token_kind token_kind, char *token_string) {
    struct tach_tokenize_token return_token;
    return_token.token_kind = token_kind;
    return_token.token_string = tach_string_copy(token_string);
    return return_token;
}

struct tach_tokenize_token tach_tokenize_read_token_as_number(FILE *input_file) {
    char _buffer[256];
    unsigned char _buffer_place = 0;
    char current_char = '\0';
    do {
        current_char = getc(input_file);
        _buffer[_buffer_place] = current_char;
        _buffer_place ++;
    } while (current_char >= '0' && current_char <= '9');
    ungetc(current_char, input_file);
    _buffer[_buffer_place-1] = '\0';
    struct tach_tokenize_token this_token = tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_INTEGER, _buffer);
    return this_token;
}

struct tach_tokenize_token tach_tokenize_read_token_as_string(FILE *input_file) {
    char _buffer[256];
    unsigned char _buffer_place = 0;
    char current_char = getc(input_file);
    do {
        current_char = getc(input_file);
        _buffer[_buffer_place] = current_char;
        _buffer_place ++;
    } while (current_char != '"');
    _buffer[_buffer_place-1] = '\0';
    struct tach_tokenize_token this_token = tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_STRING, _buffer);
    return this_token;
}

struct tach_tokenize_token tach_tokenize_read_token_as_identifier(FILE *input_file) {
    char ident_buffer[256];
    unsigned char ident_buffer_place = 0;
    char current_char = '\0';
    do {
        current_char = getc(input_file);
        ident_buffer[ident_buffer_place] = current_char;
        ident_buffer_place ++;
    } while ((current_char >= 'a' && current_char <= 'z')
        || (current_char >= 'A' && current_char <= 'Z')
        || (current_char >= '0' && current_char <= '9')
        || current_char == '_');
    ungetc(current_char, input_file);
    ident_buffer[ident_buffer_place-1] = '\0';
    struct tach_tokenize_token this_token = tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_IDENTIFIER, ident_buffer);
    return this_token;
}

struct tach_tokenize_token tach_tokenize_read_token(FILE *input_file) {
    char next_char = tach_tokenize_peek_pfile(input_file);
    if (next_char >= '0' && next_char <= '9') {
        return tach_tokenize_read_token_as_number(input_file);
    }
    if ((next_char >= 'a' && next_char <= 'z')
        || (next_char >= 'A' && next_char <= 'Z')
        || next_char == '_') {
        return tach_tokenize_read_token_as_identifier(input_file);
    }
    if (next_char == '"') {
        return tach_tokenize_read_token_as_string(input_file);
    }
    if (next_char == ';') {
        getc(input_file);
        return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_SEMICOLON, ";");
    }
    if (next_char == ',') {
        getc(input_file);
        return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_COMMA, ",");
    }
    if (next_char == '(') {
        getc(input_file);
        return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_OPEN, "(");
    }
    if (next_char == ')') {
        getc(input_file);
        return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_CLOSE, ")");
    }
    if (next_char == '{') {
        getc(input_file);
        return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_OPEN, "{");
    }
    if (next_char == '}') {
        getc(input_file);
        return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_CLOSE, "}");
    }
    if (next_char == '+') {
        getc(input_file);
        next_char = tach_tokenize_peek_pfile(input_file);
        if (next_char == '+') {
            getc(input_file);
            return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_OPERATOR, "++");
        }
        else if (next_char == '=') {
            getc(input_file);
            return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_OPERATOR, "+=");
        }
        else {
            return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_OPERATOR, "+");
        }
    }
    if (next_char == '-') {
        getc(input_file);
        next_char = tach_tokenize_peek_pfile(input_file);
        if (next_char == '-') {
            getc(input_file);
            return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_OPERATOR, "--");
        }
        else if (next_char == '=') {
            getc(input_file);
            return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_OPERATOR, "-=");
        }
        else {
            return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_OPERATOR, "-");
        }
    }
    if (next_char == '*') {
        getc(input_file);
        next_char = tach_tokenize_peek_pfile(input_file);
        if (next_char == '=') {
            getc(input_file);
            return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_OPERATOR, "*=");
        }
        else {
            return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_OPERATOR, "*");
        }
    }
    if (next_char == '/') {
        getc(input_file);
        next_char = tach_tokenize_peek_pfile(input_file);
        if (next_char == '=') {
            getc(input_file);
            return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_OPERATOR, "/=");
        }
        else {
            return tach_tokenize_create_token_string(TACH_TOKENIZE_TOKEN_KIND_OPERATOR, "/");
        }
    }
    char skip_char = getc(input_file);
    if (skip_char == EOF) {
        return tach_tokenize_create_token_empty(TACH_TOKENIZE_TOKEN_KIND_END_OF_FILE);
    }
    return tach_tokenize_read_token(input_file);
}