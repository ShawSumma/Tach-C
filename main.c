#include "tach.h"
#include "lexer.h"
#include "ast.h"
#include "back.h"

int main(int argc, char **argv) {
    if (argc == 1) {
        fprintf(stderr, "give a file name\n");
        exit(1);
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "cannot open file\n");
        exit(1);
    }
    struct tach_tokenize_token_group group = tach_tokenize_file_whole(file);
    fclose(file);
    struct tach_ast_node_file_program *ast = tach_ast_read_file_program(&group);
    FILE *outf = fopen("output/compiled", "w");
    struct tach_back_c_state *state = tach_back_c_state_create(outf);
    tach_back_c_program_compile(ast, state);
    fclose(outf);
}