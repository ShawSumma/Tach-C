#pragma once
#include "tach.h"
#include "lexer.h"

struct tach_ast_node_file_program;
struct tach_ast_node_function_declaration;
struct tach_ast_node_ctype;
struct tach_ast_node_identifier;
struct tach_ast_node_block;
struct tach_ast_node_statement;
struct tach_ast_node_number;
struct tach_ast_node_expression;
struct tach_ast_node_operator;
struct tach_ast_node_flow_change;
struct tach_ast_node_flow_control;
struct tach_ast_node_function_call;

enum tach_ast_node_statement_type {
    TACH_AST_NODE_STATEMENT_BLOCK = 1,
    TACH_AST_NODE_STATEMENT_FLOW_CHANGE,
    TACH_AST_NODE_STATEMENT_FLOW_CONTROL,
    TACH_AST_NODE_STATEMENT_EXPRESSION,
};

enum tach_ast_node_flow_control_type {
    TACH_AST_NODE_FLOW_CONTROL_IF = 1,
    TACH_AST_NODE_FLOW_CONTROL_WHILE,
    TACH_AST_NODE_FLOW_CONTROL_DO_WHILE,
};

enum tach_ast_node_flow_clange_type {
    TACH_AST_NODE_FLOW_CHANGE_RETURN = 1,
    TACH_AST_NODE_FLOW_CHANGE_BREAK,
    TACH_AST_NODE_FLOW_CHANGE_CONTINUE,
};

enum tach_ast_node_expression_type {
    TACH_AST_NODE_EXPRESSION_TYPE_NUMBER = 1,
    TACH_AST_NODE_EXPRESSION_TYPE_STRING,
    TACH_AST_NODE_EXPRESSION_TYPE_NAME,
    TACH_AST_NODE_EXPRESSION_TYPE_OPERATOR,
    TACH_AST_NODE_EXPRESSION_TYPE_FUNCTION_CALL,
};

enum tach_ast_operator_level {
    TACH_AST_OPERATOR_LEVEL_ADD = 0,
    TACH_AST_OPERATOR_LEVEL_MULT,
};

union tach_ast_node_statement_child {
    struct tach_ast_node_block *block_value;
    struct tach_ast_node_flow_change *flow_change_value;
    struct tach_ast_node_flow_control *flow_control_value;
    struct tach_ast_node_expression *expression_value;
};

union tach_ast_node_expression_child {
    char *string;
    struct tach_ast_node_number *number;
    struct tach_ast_node_function_call *call;
    struct tach_ast_node_operator *operator;
    char *name;
};

struct tach_ast_node_operator {
    char *oper;
    struct tach_ast_node_expression **children;
    long child_count;
};

struct tach_ast_node_file_program {
    struct tach_ast_node_function_declaration **function_decls;
    long function_decl_count;
};

struct tach_ast_node_file_program *tach_ast_read_file_program(struct tach_tokenize_token_group *tokens);
struct tach_ast_node_function_declaration *tach_ast_read_function_declaration(struct tach_tokenize_token_group *tokens);
struct tach_ast_node_identifier *tach_ast_read_identifier(struct tach_tokenize_token_group *tokens);
struct tach_ast_node_identifier *tach_ast_read_identifier_as_type(struct tach_tokenize_token_group *tokens);
struct tach_ast_node_block *tach_ast_read_block(struct tach_tokenize_token_group *tokens);
struct tach_ast_node_statement *tach_ast_read_statement(struct tach_tokenize_token_group *tokens);
struct tach_ast_node_expression *tach_ast_read_expression_single(struct tach_tokenize_token_group *tokens);
struct tach_ast_node_expression *tach_ast_read_expression(struct tach_tokenize_token_group *tokens);

struct tach_ast_node_function_declaration {
    struct tach_ast_node_ctype *return_type;
    struct tach_ast_node_identifier *global_name;
    struct tach_ast_node_ctype **argument_types;
    struct tach_ast_node_identifier **argument_names;
    struct tach_ast_node_block *code_body;
    long argument_count;
};

struct tach_ast_node_ctype {
    struct tach_ast_node_identifier **names;
    long name_count;
};

struct tach_ast_node_identifier {
    char **name;
};

struct tach_ast_node_block {
    struct tach_ast_node_statement **statements;
    long statement_count;
};

struct tach_ast_node_statement {
    enum tach_ast_node_statement_type type;
    union tach_ast_node_statement_child child;
};

struct tach_ast_node_expression {
    enum tach_ast_node_expression_type type;
    union tach_ast_node_expression_child child;
};

struct tach_ast_node_flow_control {
    enum tach_ast_node_flow_control_type flow_type;
    struct tach_ast_node_expression *test;
    struct tach_ast_node_block *flow_body;
};

struct tach_ast_node_flow_change {
    enum tach_ast_node_flow_clange_type type;
    struct tach_ast_node_expression *expression;
};

struct tach_ast_node_number {
    char *rep_value;
};

struct tach_ast_node_function_call {
    struct tach_ast_node_expression *function;
    struct tach_ast_node_expression **arguments;
    long argument_count;
};
