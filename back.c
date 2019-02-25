#include "back.h"

void tach_back_c_program_compile(struct tach_ast_node_file_program *program, struct tach_back_c_state *state) {
    fprintf(state->file, "#!/usr/bin/env lua5.1\n");
    fprintf(state->file, "require(\"output/lib\")\n");
    for (long i = 0; i < program->function_decl_count; i++) {
        tach_back_c_function_compile(program->function_decls[i], state);
    }
    fprintf(state->file, "main()");
}

void tach_back_c_function_compile(struct tach_ast_node_function_declaration *program, struct tach_back_c_state *state) {
    fprintf(state->file, "function ");
    fprintf(state->file, "%s", program->global_name->name[0]);
    fprintf(state->file, "(");
    for (long i = 0; i < program->argument_count; i++) {
        if (i != 0) {
            fprintf(state->file, ", ");
        }
        fprintf(state->file, "%s", program->argument_names[i]->name[0]);
    }
    fprintf(state->file, ")");
    tach_back_c_state_p_newline(state);
    tach_back_c_block_compile(program->code_body, state);
    tach_back_c_state_m_newline(state);
    fprintf(state->file, "end");
    tach_back_c_state_newline(state);
}

void tach_back_c_block_compile(struct tach_ast_node_block *ast, struct tach_back_c_state *state) {
    fprintf(state->file, "do");
    tach_back_c_state_p_newline(state);
    for (long i = 0; i < ast->statement_count; i++) {
        tach_back_c_statement_compile(ast->statements[i], state);
        if (i+1 !=  ast->statement_count) {
            tach_back_c_state_newline(state);
        }
    }
    tach_back_c_state_m_newline(state);
    fprintf(state->file, "end");
}

void tach_back_c_statement_compile(struct tach_ast_node_statement *ast, struct tach_back_c_state *state) {
    switch (ast->type) {
        case TACH_AST_NODE_STATEMENT_BLOCK: {
            tach_back_c_block_compile(ast->child.block_value, state);
            break;
        }
        case TACH_AST_NODE_STATEMENT_EXPRESSION: {
            tach_back_c_expression_compile(ast->child.expression_value, state);
            break;
        }
        case TACH_AST_NODE_STATEMENT_FLOW_CHANGE: {
            tach_back_c_flow_change_compile(ast->child.flow_change_value, state);
            break;
        }
        case TACH_AST_NODE_STATEMENT_FLOW_CONTROL: {
            tach_back_c_flow_control_compile(ast->child.flow_control_value, state);
            break;
        }
    }
}

void tach_back_c_flow_change_compile(struct tach_ast_node_flow_change *ast, struct tach_back_c_state *state) {
    switch (ast->type) {
        case TACH_AST_NODE_FLOW_CHANGE_RETURN: {
            fprintf(state->file, "return ");          
            tach_back_c_expression_compile(ast->expression, state);
            break;
        }
        case TACH_AST_NODE_FLOW_CHANGE_BREAK: {
            fprintf(state->file, "break");          
            break;
        }
        case TACH_AST_NODE_FLOW_CHANGE_CONTINUE: {
            fprintf(state->file, "continue");          
            break;
        }
    }
}

void tach_back_c_flow_control_compile(struct tach_ast_node_flow_control *ast, struct tach_back_c_state *state) {

}

void tach_back_c_expression_compile(struct tach_ast_node_expression *ast, struct tach_back_c_state *state) {
    switch (ast->type) {
        case TACH_AST_NODE_EXPRESSION_TYPE_NAME: {
            fprintf(state->file, "%s", ast->child.name);        
            break; 
        }
        case TACH_AST_NODE_EXPRESSION_TYPE_NUMBER: {
            fprintf(state->file, "%s", ast->child.number->rep_value);
            break;
        }
        case TACH_AST_NODE_EXPRESSION_TYPE_OPERATOR: {
            tach_back_c_expression_compile(ast->child.operator->children[0], state);
            fprintf(state->file, "%s", ast->child.operator->oper);
            tach_back_c_expression_compile(ast->child.operator->children[1], state);
            break;
        }
        case TACH_AST_NODE_EXPRESSION_TYPE_STRING: {
            fprintf(state->file, "\"%s\"", ast->child.string);
            break;
        }
        case TACH_AST_NODE_EXPRESSION_TYPE_FUNCTION_CALL: {
            tach_back_c_expression_compile(ast->child.call->function, state);
            fprintf(state->file, "(");
            for (long i = 0; i < ast->child.call->argument_count; i++) {
                if (i != 0) {
                    fprintf(state->file, ", ");
                }
                tach_back_c_expression_compile(ast->child.call->arguments[i], state);
            }
            fprintf(state->file, ")");
            break;
        }
    }
}

void tach_back_c_state_newline(struct tach_back_c_state *state) {
    fprintf(state->file, "\n");
    for (int i = 0; i < state->depth; i++) {
        fprintf(state->file, "  ");
    }
}

void tach_back_c_state_p_newline(struct tach_back_c_state *state) {
    fprintf(state->file, "\n");
    state->depth ++;
    for (int i = 0; i < state->depth; i++) {
        fprintf(state->file, "  ");
    }
}

void tach_back_c_state_m_newline(struct tach_back_c_state *state) {
    fprintf(state->file, "\n");
    state->depth --;
    for (int i = 0; i < state->depth; i++) {
        fprintf(state->file, "  ");
    }
}

struct tach_back_c_state *tach_back_c_state_create(FILE *file) {
    struct tach_back_c_state *ret = tach_malloc(sizeof(struct tach_back_c_state));
    ret->depth = 0;
    ret->file = file;
    return ret;
}