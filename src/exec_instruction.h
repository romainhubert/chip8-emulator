#ifndef EXEC_INSTRUCTION_H
#define EXEC_INSTRUCTION_H

#include "chip8.h"

void execute_instruction(struct chip8* state, struct decoded_instruction instruction);

#endif // EXEC_INSTRUCTION_H
