#pragma once
#include "tach.h"

struct tach_tokenize_token;
struct tach_tokenize_token_group;
enum tach_tokenize_token_kind;

struct tach_tokenize_token tach_tokenize_read_token(FILE *input_file);
struct tach_tokenize_token tach_tokenize_read_token_as_number(FILE *input_file);
struct tach_tokenize_token tach_tokenize_read_token_as_identifier(FILE *input_file);
struct tach_tokenize_token tach_tokenize_read_token_as_string(FILE *input_file);
struct tach_tokenize_token tach_tokenize_create_token_empty(enum tach_tokenize_token_kind token_kind);
struct tach_tokenize_token tach_tokenize_create_token_string(enum tach_tokenize_token_kind token_kind, char *token_string);
struct tach_tokenize_token_group tach_tokenize_file_until(FILE *input_file, char end_of_tokens);
struct tach_tokenize_token_group tach_tokenize_file_whole(FILE *input_file);
char *tach_create_copy_string(char *old_string);

enum tach_tokenize_token_kind {
    TACH_TOKENIZE_TOKEN_KIND_END_OF_FILE = 0,
    TACH_TOKENIZE_TOKEN_KIND_IDENTIFIER,
    TACH_TOKENIZE_TOKEN_KIND_INTEGER,
    TACH_TOKENIZE_TOKEN_KIND_FLOATING,
    TACH_TOKENIZE_TOKEN_KIND_STRING,
    TACH_TOKENIZE_TOKEN_KIND_OPEN,
    TACH_TOKENIZE_TOKEN_KIND_CLOSE,
    TACH_TOKENIZE_TOKEN_KIND_SEMICOLON,
    TACH_TOKENIZE_TOKEN_KIND_COMMA,
    TACH_TOKENIZE_TOKEN_KIND_OPERATOR,
};

struct tach_tokenize_token {
    char *token_string;
    enum tach_tokenize_token_kind token_kind;
};

struct tach_tokenize_token_group {
    struct tach_tokenize_token *tokens;
    long count_tokens;
    long memory_count_tokens;
    long token_index;
};