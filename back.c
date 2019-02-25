#include "back.h"

void tach_back_c_program_compile(struct tach_inter_program *program, FILE *output) {
    fprintf(output, "#!/usr/bin/env luajit\n");
    fprintf(output, "require(\"output/_tach\")\n");
    for (long i = 0; i < program->function_count; i++) {
        struct tach_inter_function *function = program->functions[i];
        fprintf(output, "function gen_%s(...)\n", program->names[i]->name[0]);
        fprintf(output, "  local arg = {...}\n");
        fprintf(output, "  local stack = {}\n");
        fprintf(output, "  local retv = 0\n");
        tach_back_c_function_compile(function, output);
        fprintf(output, "  ::ret::\n");
        fprintf(output, "  return retv\n");
        fprintf(output, "end\n");
    }
    fprintf(output, "os.exit(gen_main(#arg, arg))");
}

void tach_back_c_type_compile(struct tach_inter_type *opcode, FILE *output) {
    fprintf(output, "%s", opcode->type);
}

void tach_back_c_opcode_compile(struct tach_inter_opcode *opcode, FILE *output) {
    switch (opcode->kind) {
        case TACH_INTER_OPCODE_KIND_ERROR: {
            fprintf(stderr, "bad bytecode generation\n");
            exit(1);
        }
        case TACH_INTER_OPCODE_KIND_LOAD_ARGS: {
            struct tach_ast_node_identifier **names = opcode->any_value;
            for (long i = 0; i < opcode->index; i++) {
                fprintf(output, "  local gen_%s = arg[%ld]\n", names[i]->name[0], i+1);
            }
            return;
        }
        case TACH_INTER_OPCODE_KIND_NOP: {
            return;
        }
        case TACH_INTER_OPCODE_KIND_RETURN: {
            fprintf(output, "  retv = stack[#stack]\n  goto ret\n");        
            return;
        };
        case TACH_INTER_OPCODE_KIND_PUSH_INT: {
            fprintf(output, "  stack[#stack+1] = %d\n", opcode->index);        
            return;
        }
        case TACH_INTER_OPCODE_KIND_PUSH_STRING: {
            fprintf(output, "  stack[#stack+1] = \"%s\"\n", (char *) opcode->any_value);        
            return;
        }
        case TACH_INTER_OPCODE_KIND_PUSH_NAME: {
            fprintf(output, "  stack[#stack+1] = gen_%s\n", (char*) opcode->any_value);        
            return;
        }
        case TACH_INTER_OPCODE_KIND_STORE_NAME: {
            fprintf(output, "  local gen_%s = stack[#stack+1]\n", (char*) opcode->any_value);        
            return;
        }
        case TACH_INTER_OPCODE_KIND_CALL: {
            fprintf(output, "  do\n");
            fprintf(output, "    local args = {}\n");
            fprintf(output, "    local argno = %u\n", opcode->index);
            fprintf(output, "    while argno > 0 do\n");
            fprintf(output, "      args[argno] = stack[#stack]\n");
            fprintf(output, "      stack[#stack] = nil\n");
            fprintf(output, "      argno = argno - 1\n");
            fprintf(output, "    end\n");
            fprintf(output, "    local func = stack[#stack]\n");
            fprintf(output, "    stack[#stack] = nil\n");
            fprintf(output, "    local result = func(unpack(args))\n");        
            fprintf(output, "    stack[#stack+1] = result\n");  
            fprintf(output, "  end\n");
            return;
        }
        case TACH_INTER_OPCODE_KIND_OPERATOR: {
            char *opcode_name = NULL;
            char *opcode_raw = tach_string_copy(opcode->any_value);
            if (opcode_raw[0] == '+') {
                if (opcode_raw[1] == '\0') {
                    opcode_name = "_op_add";
                }
            }
            if (opcode_raw[0] == '*') {
                if (opcode_raw[1] == '\0') {
                    opcode_name = "_op_mul";
                }
            }
            if (opcode_raw[0] == '/') {
                if (opcode_raw[1] == '\0') {
                    opcode_name = "_op_div";
                }
            }
            if (opcode_raw[0] == '-') {
                if (opcode_raw[1] == '\0') {
                    opcode_name = "_op_subl";
                }
            }
            if (opcode_name == NULL) {
                fprintf(stderr, "operator unsupported %s\n", opcode_raw);
                exit(1);
            }
            fprintf(output, "  do\n");
            fprintf(output, "    local args = {}\n");
            fprintf(output, "    for i=1, %u do\n", opcode->index);
            fprintf(output, "      args[%u-i] = stack[#stack]\n", opcode->index+1);
            fprintf(output, "      stack[#stack] = nil\n");
            fprintf(output, "    end\n");
            fprintf(output, "    local result = gen_%s(unpack(args))\n", opcode_name);        
            fprintf(output, "    stack[#stack+1] = result\n");  
            fprintf(output, "  end\n");
            return;            
        }
        default: {
            fprintf(stderr, "bytecode %d needs to be handled\n", opcode->kind);
            exit(1);
        }
    }
}

void tach_back_c_function_compile(struct tach_inter_function *function, FILE *output) {
    struct tach_inter_opcode *current_opcode = function->entry;
    while (current_opcode != NULL) {
        tach_back_c_opcode_compile(current_opcode, output);
        current_opcode = current_opcode->next;
    }
}