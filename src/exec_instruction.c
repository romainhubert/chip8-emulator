#include "chip8.h"
#include <stdio.h>

static void clear_display(struct chip8* state){
    for(int y = 0; y < 32; y++){
        for(int x = 0; x < 64; x++){
            state->display[y][x] = 0;
        }
    }
    printf("cleared display");
    state->draw_flag = 1;
}

void execute_instruction(struct chip8* state, struct decoded_instruction instruction){
    printf("%04X\n", instruction.instruction);
    printf("opcode=%04X\n", instruction.opcode);
    printf("X=%04X\n", instruction.X);
    printf("Y=%04X\n", instruction.Y);
    printf("N=%04X\n", instruction.N);
    printf("NN=%04X\n", instruction.NN);
    printf("NNN=%04X\n", instruction.NNN);
    switch(instruction.opcode){
        case 0:
            if(instruction.NNN == 0x0E0){
                clear_display(state);
            }
            break;
    }
    state->memory[0]++;
}
