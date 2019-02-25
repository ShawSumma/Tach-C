#include "walk.h"

struct tach_inter_program *tach_inter_conv_program(struct tach_ast_node_file_program *program) {
    struct tach_inter_program *return_program = tach_malloc(sizeof(struct tach_inter_program));
    return_program->names = tach_malloc(sizeof(struct tach_ast_node_identifier *) * program->function_decl_count);
    return_program->functions = tach_malloc(sizeof(struct tach_inter_function *) * program->function_decl_count);
    return_program->function_count = 0;
    for (long i = 0; i < program->function_decl_count; i++) {
        return_program->functions[i] = tach_inter_conv_function(program->function_decls[i]);
        return_program->names[i] = program->function_decls[i]->global_name;
        return_program->function_count ++;
    }
    return return_program;
}

struct tach_inter_function *tach_inter_conv_function(struct tach_ast_node_function_declaration *function) {
    struct tach_inter_function *return_function = tach_malloc(sizeof(struct tach_inter_function));
    return_function->arg_count = function->argument_count;
    return_function->arg_names = tach_malloc(sizeof(char *)*function->argument_count);
    return_function->arg_types = tach_malloc(sizeof(struct tach_inter_type *)*function->argument_count);
    for (long i = 0; i < return_function->arg_count; i++) {
        return_function->arg_names[i] = function->argument_names[i]->name[0];
        return_function->arg_types[i] = tach_inter_conv_type(function->argument_types[i]);
    }
    return_function->return_type = tach_inter_conv_type(function->return_type);

    struct tach_inter_opcode *begin = tach_malloc(sizeof(struct tach_inter_opcode));
    begin->kind = TACH_INTER_OPCODE_KIND_LOAD_ARGS;
    begin->index = function->argument_count;
    begin->any_value = function->argument_names;
    struct tach_inter_opcode *end = tach_inter_conv_block(function->code_body, begin);
    end->next = NULL;
    return_function->entry = begin;
    return return_function;
}

struct tach_inter_type *tach_inter_conv_type(struct tach_ast_node_ctype *ctype) {
    struct tach_inter_type *return_type = tach_malloc(sizeof(struct tach_inter_type));
    return_type->type = tach_string_copy(ctype->names[0]->name[0]);
    return return_type;
}

struct tach_inter_opcode *tach_inter_conv_flow_change(struct tach_ast_node_flow_change *change, struct tach_inter_opcode *last) {
    switch (change->type) {
        case TACH_AST_NODE_FLOW_CHANGE_RETURN: {
            struct tach_inter_opcode *first = tach_inter_conv_expression(change->expression, last);
            struct tach_inter_opcode *return_opcode = tach_malloc(sizeof(struct tach_inter_opcode));
            return_opcode->kind = TACH_INTER_OPCODE_KIND_RETURN;
            return_opcode->index = 0;
            return_opcode->next = NULL;
            first->next = return_opcode;
            return return_opcode;
        }
        default : {
            fprintf(stderr, "error unhandled in tach_inter_conv_flow_change\n");
            exit(1);
        }
    }
}

struct tach_inter_opcode *tach_inter_conv_number(struct tach_ast_node_number *number, struct tach_inter_opcode *last) {
    struct tach_inter_opcode *number_opcode = tach_malloc(sizeof(struct tach_inter_opcode));
    number_opcode->kind = TACH_INTER_OPCODE_KIND_PUSH_INT;
    number_opcode->index = atoi(number->rep_value);
    number_opcode->next = NULL;
    last->next = number_opcode;
    return number_opcode;
}

struct tach_inter_opcode *tach_inter_conv_string(char *string, struct tach_inter_opcode *last) {
    struct tach_inter_opcode *string_opcode = tach_malloc(sizeof(struct tach_inter_opcode));
    string_opcode->kind = TACH_INTER_OPCODE_KIND_PUSH_STRING;
    string_opcode->index = 0;
    string_opcode->any_value = string;
    string_opcode->next = NULL;
    last->next = string_opcode;
    return string_opcode;
}

struct tach_inter_opcode *tach_inter_conv_expression(struct tach_ast_node_expression *expr, struct tach_inter_opcode *last) {
    switch (expr->type) {
        case TACH_AST_NODE_EXPRESSION_TYPE_OPERATOR: {
            for (long i = 0; i < expr->child.operator->child_count; i++) {
                last = tach_inter_conv_expression(expr->child.operator->children[i], last);
            }
            struct tach_inter_opcode *oper_opcode = tach_malloc(sizeof(struct tach_inter_opcode));
            oper_opcode->kind = TACH_INTER_OPCODE_KIND_OPERATOR;
            oper_opcode->any_value = (void*) tach_string_copy(expr->child.operator->oper);
            oper_opcode->index = expr->child.operator->child_count;
            oper_opcode->next = NULL;
            last->next = oper_opcode;
            return oper_opcode;
        }
        case TACH_AST_NODE_EXPRESSION_TYPE_NUMBER: {
            return tach_inter_conv_number(expr->child.number, last);
        }
        case TACH_AST_NODE_EXPRESSION_TYPE_STRING: {
            return tach_inter_conv_string(expr->child.string, last);
        }
        case TACH_AST_NODE_EXPRESSION_TYPE_FUNCTION_CALL: {
            last = tach_inter_conv_expression(expr->child.call->function, last);
            struct tach_inter_opcode *current = last;
            for (long i = 0; i < expr->child.call->argument_count; i++) {
                current = tach_inter_conv_expression(expr->child.call->arguments[i], current);
            }
            last = current;
            struct tach_inter_opcode *call_opcode = tach_malloc(sizeof(struct tach_inter_opcode));
            call_opcode->kind = TACH_INTER_OPCODE_KIND_CALL;
            call_opcode->index = expr->child.call->argument_count;
            call_opcode->next = NULL;
            last->next = call_opcode;
            return call_opcode;
        }
        case TACH_AST_NODE_EXPRESSION_TYPE_NAME: {
            struct tach_inter_opcode *name_opcode = tach_malloc(sizeof(struct tach_inter_opcode));
            name_opcode->kind = TACH_INTER_OPCODE_KIND_PUSH_NAME;
            name_opcode->index = 0;
            name_opcode->any_value = tach_string_copy(expr->child.name);
            name_opcode->next = NULL;
            last->next = name_opcode;
            return name_opcode;
        }
        default: {
            fprintf(stderr, "ast expression error\n");
            exit(1);
        } 
    }
}

struct tach_inter_opcode *tach_inter_conv_statement(struct tach_ast_node_statement *state, struct tach_inter_opcode *last) {
    switch (state->type) {
        case TACH_AST_NODE_STATEMENT_BLOCK: {
            return tach_inter_conv_block(state->child.block_value, last);
        }
        case TACH_AST_NODE_STATEMENT_FLOW_CHANGE: {
            return tach_inter_conv_flow_change(state->child.flow_change_value, last);
        }
        case TACH_AST_NODE_STATEMENT_EXPRESSION: {
            return tach_inter_conv_expression(state->child.expression_value, last);
        }
        default : {
            fprintf(stderr, "error unhandled in tach_inter_conv_statement\n");
            exit(1);
        }
    }
}

struct tach_inter_opcode *tach_inter_conv_block(struct tach_ast_node_block *block, struct tach_inter_opcode *last) {
    struct tach_inter_opcode *current = last;
    for (long i = 0; i < block->statement_count; i++) {
        current = tach_inter_conv_statement(block->statements[i], current);
    }
    current->next = NULL;
    return current;
}