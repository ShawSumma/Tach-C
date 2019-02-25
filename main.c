#include "tach.h"
#include "lexer.h"
#include "ast.h"
#include "walk.h"
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
    // for (long i = 0; i < group.count_tokens; i++) {
    //     puts(group.tokens[i].token_string);
    // }
    struct tach_ast_node_file_program *ast = tach_ast_read_file_program(&group);
    struct tach_inter_program *program = tach_inter_conv_program(ast);
    // struct tach_inter_opcode *bc = program->functions[0]->entry;
    // while (bc != NULL) {
    //     printf("%d\t%d\n", bc->kind, bc->index);
    //     bc = bc->next;
    // }
    FILE *outf = fopen("output/compiled", "w");
    tach_back_c_program_compile(program, outf);
    fclose(outf);
}