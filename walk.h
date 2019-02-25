#pragma once
#include "tach.h"
#include "ast.h"

struct tach_inter_opcode;
struct tach_inter_function;
struct tach_inter_program;
struct tach_inter_type;

enum tach_inter_opcode_kind {
    TACH_INTER_OPCODE_KIND_ERROR = 0,
    TACH_INTER_OPCODE_KIND_NOP = 1,
    TACH_INTER_OPCODE_KIND_LOAD_ARGS,
    TACH_INTER_OPCODE_KIND_JUMP,
    TACH_INTER_OPCODE_KIND_SKIP,
    TACH_INTER_OPCODE_KIND_PUSH_INT,
    TACH_INTER_OPCODE_KIND_PUSH_STRING,
    TACH_INTER_OPCODE_KIND_PUSH_NAME,
    TACH_INTER_OPCODE_KIND_STORE_NAME, 
    TACH_INTER_OPCODE_KIND_RETURN,
    TACH_INTER_OPCODE_KIND_CONTINUE,
    TACH_INTER_OPCODE_KIND_BREAK,
    TACH_INTER_OPCODE_KIND_CALL,
    TACH_INTER_OPCODE_KIND_OPERATOR,
};

struct tach_inter_type {
    char *type;
};

struct tach_inter_program {
    struct tach_inter_function **functions;
    struct tach_ast_node_identifier **names;
    long function_count;
};

struct tach_inter_function {
    struct tach_inter_type *return_type;
    struct tach_inter_type **arg_types;
    struct tach_inter_opcode *entry;
    long arg_count;
    char **arg_names;
};

struct tach_inter_opcode {
    struct tach_inter_opcode *next;
    void *any_value;
    unsigned int index;
    enum tach_inter_opcode_kind kind;
};

struct tach_inter_program *tach_inter_conv_program(struct tach_ast_node_file_program *program);
struct tach_inter_function *tach_inter_conv_function(struct tach_ast_node_function_declaration *);
struct tach_inter_type *tach_inter_conv_type(struct tach_ast_node_ctype *ctype);
struct tach_inter_opcode *tach_inter_conv_identifier(struct tach_ast_node_identifier *ident, struct tach_inter_opcode *last);
struct tach_inter_opcode *tach_inter_conv_block(struct tach_ast_node_block *block, struct tach_inter_opcode *last);
struct tach_inter_opcode *tach_inter_conv_statement(struct tach_ast_node_statement *state, struct tach_inter_opcode *last);
struct tach_inter_opcode *tach_inter_conv_number(struct tach_ast_node_number *number, struct tach_inter_opcode *last);
struct tach_inter_opcode *tach_inter_conv_string(char *string, struct tach_inter_opcode *last);
struct tach_inter_opcode *tach_inter_conv_expression(struct tach_ast_node_expression *expr, struct tach_inter_opcode *last);
struct tach_inter_opcode *tach_inter_conv_flow_change(struct tach_ast_node_flow_change *change, struct tach_inter_opcode *last);
struct tach_inter_opcode *tach_inter_conv_flow_control(struct tach_ast_node_flow_control *control, struct tach_inter_opcode *last);
