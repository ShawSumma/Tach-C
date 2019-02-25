#include "ast.h"

struct tach_ast_node_file_program *tach_ast_read_file_program(struct tach_tokenize_token_group *tokens) {
    struct tach_ast_node_file_program *return_program = tach_malloc(sizeof(struct tach_ast_node_file_program));
    return_program->function_decl_count = 0;
    long memcount = 10;
    return_program->function_decls = tach_malloc(sizeof(struct tach_ast_node_function_declaration*)*10);
    while (tokens->token_index + 1 < tokens->count_tokens) {
        if (memcount >= return_program->function_decl_count) {
            memcount *= 1.5;
            return_program->function_decls = tach_realloc(return_program->function_decls, sizeof(struct tach_ast_node_function_declaration*)*memcount);
        }
        return_program->function_decls[return_program->function_decl_count] = (struct tach_ast_node_function_declaration *)
            tach_ast_read_function_declaration(tokens);
        return_program->function_decl_count ++;
    }
    return return_program;
}

struct tach_ast_node_function_declaration *tach_ast_read_function_declaration(struct tach_tokenize_token_group *tokens) {
    struct tach_ast_node_function_declaration *return_declaration = tach_malloc(sizeof(struct tach_ast_node_function_declaration));
    struct tach_ast_node_ctype *function_return_type = tach_malloc(sizeof(struct tach_ast_node_ctype));
    function_return_type->name_count = 1;
    function_return_type->names = tach_malloc(sizeof(struct tach_ast_node_identifier *)*1);
    function_return_type->names[0] = tach_ast_read_identifier_as_type(tokens);
    return_declaration->return_type = function_return_type;
    return_declaration->global_name = tach_ast_read_identifier(tokens);
    tokens->token_index ++;

    long current_place = tokens->token_index;
    return_declaration->argument_count = 0;
    char is_args = 0;
    while (tokens->tokens[current_place].token_kind != TACH_TOKENIZE_TOKEN_KIND_CLOSE) {
        if (tokens->tokens[current_place].token_kind == TACH_TOKENIZE_TOKEN_KIND_COMMA) {
            return_declaration->argument_count ++;
        }
        current_place ++;
        is_args = 1;
    }
    if (is_args) {
        return_declaration->argument_count ++;
    }
    else {
        tokens->token_index += 1;
    }
    return_declaration->argument_types = tach_malloc(sizeof(struct tach_ast_node_ctype *)*return_declaration->argument_count);
    return_declaration->argument_names = tach_malloc(sizeof(struct tach_ast_node_identifier *)*return_declaration->argument_count);
    for (long i = 0; i < return_declaration->argument_count; i++) {
        return_declaration->argument_types[i] = tach_malloc(sizeof(struct tach_ast_node_ctype));
        return_declaration->argument_types[i]->name_count = 1;
        return_declaration->argument_types[i]->names = tach_malloc(sizeof(struct tach_ast_node_identifier *)*1);
        return_declaration->argument_types[i]->names[0] = tach_ast_read_identifier_as_type(tokens);
        return_declaration->argument_names[i] = tach_ast_read_identifier(tokens);
        tokens->token_index ++;
    }
    
    return_declaration->code_body = tach_ast_read_block(tokens);
    return return_declaration;
}

struct tach_ast_node_identifier *tach_ast_read_identifier(struct tach_tokenize_token_group *tokens) {
    struct tach_ast_node_identifier *return_identifier = tach_malloc(sizeof(struct tach_ast_node_identifier));
    return_identifier->name = tach_malloc(sizeof(char*)*1);
    return_identifier->name[0] = tach_string_copy(tokens->tokens[tokens->token_index].token_string);
    tokens->token_index ++;
    return return_identifier;
}

struct tach_ast_node_identifier *tach_ast_read_identifier_as_type(struct tach_tokenize_token_group *tokens) {
    long start_index = tokens->token_index;

    long count = 1;
    tokens->token_index ++;
    while (strcmp(tokens->tokens[tokens->token_index].token_string, "*") == 0) {
        tokens->token_index ++;
        count ++;
    }

    tokens->token_index = start_index;
    struct tach_ast_node_identifier *return_identifier = tach_malloc(sizeof(struct tach_ast_node_identifier));
    return_identifier->name = tach_malloc(sizeof(char*)*count);
    for (long i = 0; i < count; i++) {
        return_identifier->name[i] = tach_string_copy(tokens->tokens[tokens->token_index].token_string);
        tokens->token_index ++;
    }
    return return_identifier;
}

struct tach_ast_node_block *tach_ast_read_block(struct tach_tokenize_token_group *tokens) {
    struct tach_ast_node_block *return_block = tach_malloc(sizeof(struct tach_ast_node_block));
    if (tokens->tokens[tokens->token_index].token_kind == TACH_TOKENIZE_TOKEN_KIND_OPEN) {
        return_block->statement_count = 0;
        return_block->statements = tach_malloc(sizeof(struct tach_ast_node_statement*)*0);
        tokens->token_index ++;
        while (tokens->tokens[tokens->token_index].token_kind != TACH_TOKENIZE_TOKEN_KIND_CLOSE) {
            return_block->statements = tach_realloc(return_block->statements, sizeof(struct tach_ast_node_statement*)*(return_block->statement_count+1));
            return_block->statements[return_block->statement_count] = tach_ast_read_statement(tokens);
            return_block->statement_count ++;
        }
        tokens->token_index ++;
    }
    else {
        fprintf(stderr, "%s\n", tokens->tokens[tokens->token_index].token_string);
        fprintf(stderr, "no non block statements yet\n");
        exit(1);
        // return_block->statement_count = 1;
        // return_block->statements = tach_malloc(sizeof(struct tach_ast_node_statement*)*1);
        // return_block->statements[0] = tach_ast_read_statement(tokens);
    }
    return return_block;
}

struct tach_ast_node_statement *tach_ast_read_statement(struct tach_tokenize_token_group *tokens) {
    struct tach_ast_node_statement *return_statement = tach_malloc(sizeof(struct tach_ast_node_statement));
    struct tach_tokenize_token first = tokens->tokens[tokens->token_index];
    if (first.token_kind == TACH_TOKENIZE_TOKEN_KIND_IDENTIFIER) {
        if (strcmp(first.token_string, "return") == 0) {
            tokens->token_index ++;
            return_statement->type = TACH_AST_NODE_STATEMENT_FLOW_CHANGE;
            struct tach_ast_node_flow_change *flow_change = tach_malloc(sizeof(struct tach_ast_node_flow_change));
            flow_change->expression = tach_ast_read_expression(tokens);
            flow_change->type = TACH_AST_NODE_FLOW_CHANGE_RETURN;
            return_statement->child.flow_change_value = flow_change;
            tokens->token_index ++;
        }
        else if (strcmp(first.token_string, "if") == 0) {
            tokens->token_index += 2;
            return_statement->type = TACH_AST_NODE_STATEMENT_FLOW_CONTROL;
            return_statement->child.flow_control_value = tach_malloc(sizeof(struct tach_ast_node_flow_control));
            return_statement->child.flow_control_value->flow_type = TACH_AST_NODE_FLOW_CONTROL_IF;
            return_statement->child.flow_control_value->test = tach_ast_read_expression(tokens);
            tokens->token_index ++;
            return_statement->child.flow_control_value->flow_body = tach_ast_read_block(tokens);
        }
        else {
            return_statement->type = TACH_AST_NODE_STATEMENT_EXPRESSION;
            return_statement->child.expression_value = tach_ast_read_expression(tokens);
            tokens->token_index ++;
        }
    }
    else {
        // return_statement->type = TACH_AST_NODE_STATEMENT_EXPRESSION;
        // return_statement->child.expression_value = tach_ast_read_expression(tokens);
        fprintf(stderr, "expected statement\n");
        exit(1);
    }
    return return_statement;
}

struct tach_ast_node_expression *tach_ast_read_expression_single(struct tach_tokenize_token_group *tokens) {
    struct tach_ast_node_expression *return_expression = tach_malloc(sizeof(struct tach_ast_node_expression));
    struct tach_tokenize_token first = tokens->tokens[tokens->token_index];
    if (first.token_kind == TACH_TOKENIZE_TOKEN_KIND_INTEGER) {
        tokens->token_index ++;
        return_expression->type = TACH_AST_NODE_EXPRESSION_TYPE_NUMBER;
        struct tach_ast_node_number *child_number = tach_malloc(sizeof(struct tach_ast_node_number));
        child_number->rep_value = tach_string_copy(first.token_string);
        return_expression->child.number = child_number;
    }
    if (first.token_kind == TACH_TOKENIZE_TOKEN_KIND_STRING) {
        tokens->token_index ++;
        return_expression->type = TACH_AST_NODE_EXPRESSION_TYPE_STRING;
        return_expression->child.string = tach_string_copy(first.token_string);
    }
    else if (first.token_kind == TACH_TOKENIZE_TOKEN_KIND_IDENTIFIER) {
        tokens->token_index ++;
        return_expression->type = TACH_AST_NODE_EXPRESSION_TYPE_NAME;
        return_expression->child.name = tach_string_copy(first.token_string);
        struct tach_ast_node_number *child_number = tach_malloc(sizeof(struct tach_ast_node_number));
    }
    while (tokens->tokens[tokens->token_index].token_kind == TACH_TOKENIZE_TOKEN_KIND_OPEN) {
        tokens->token_index ++;
        struct tach_ast_node_expression *call_expression = return_expression;
        struct tach_ast_node_function_call *call = tach_malloc(sizeof(struct tach_ast_node_function_call));
        call->function = call_expression;
        call->argument_count = 0;
        long memcount = 4;
        call->arguments = tach_malloc(sizeof(struct tach_ast_node_expression *)*memcount);
        while (tokens->tokens[tokens->token_index].token_kind != TACH_TOKENIZE_TOKEN_KIND_CLOSE) {
            if (tokens->tokens[tokens->token_index].token_kind == TACH_TOKENIZE_TOKEN_KIND_COMMA) {
                tokens->token_index ++;
            }
            if (memcount >= call->argument_count) {
                memcount *= 1.5;
                call->arguments = tach_realloc(call->arguments, sizeof(struct tach_ast_node_expression *)*memcount);
            }
            call->arguments[call->argument_count] = tach_ast_read_expression(tokens);
            call->argument_count ++;
        }
        return_expression = tach_malloc(sizeof(struct tach_ast_node_expression));
        return_expression->type = TACH_AST_NODE_EXPRESSION_TYPE_FUNCTION_CALL;
        return_expression->child.call = call;
        tokens->token_index ++;
    }
    return return_expression;
}

struct tach_ast_node_expression *tach_ast_read_expression(struct tach_tokenize_token_group *tokens) {  
    long begin = tokens->token_index;
    struct tach_ast_node_expression *return_expression = tach_ast_read_expression_single(tokens);
    if (tokens->tokens[tokens->token_index].token_kind == TACH_TOKENIZE_TOKEN_KIND_OPERATOR) {
        tokens->token_index ++;
    }
    return return_expression;
}