#include "tach.h"
#include "ast.h"

struct tach_back_c_state;

void tach_back_c_program_compile(struct tach_ast_node_file_program *ast, struct tach_back_c_state *state);
void tach_back_c_function_compile(struct tach_ast_node_function_declaration *ast, struct tach_back_c_state *state);
void tach_back_c_block_compile(struct tach_ast_node_block *ast, struct tach_back_c_state *state);
void tach_back_c_statement_compile(struct tach_ast_node_statement *ast, struct tach_back_c_state *state);
void tach_back_c_expression_compile(struct tach_ast_node_expression *ast, struct tach_back_c_state *state);
void tach_back_c_flow_change_compile(struct tach_ast_node_flow_change *ast, struct tach_back_c_state *state);
void tach_back_c_flow_control_compile(struct tach_ast_node_flow_control *ast, struct tach_back_c_state *state);

void tach_back_c_state_newline(struct tach_back_c_state *state);
void tach_back_c_state_p_newline(struct tach_back_c_state *state);
void tach_back_c_state_m_newline(struct tach_back_c_state *state);
struct tach_back_c_state *tach_back_c_state_create(FILE *);

struct tach_back_c_state {
    int depth;
    FILE *file;
};