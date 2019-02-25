#pragma once
#include "tach.h"
#include "walk.h"

void tach_back_c_program_compile(struct tach_inter_program *program, FILE *output);
void tach_back_c_function_compile(struct tach_inter_function *function, FILE *output);
void tach_back_c_opcode_compile(struct tach_inter_opcode *opcode, FILE *output);
void tach_back_c_type_compile(struct tach_inter_type *opcode, FILE *output);