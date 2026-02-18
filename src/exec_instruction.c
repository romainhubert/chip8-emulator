#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>

static void clear_display(struct chip8* state){
    for(int y = 0; y < 32; y++){
        for(int x = 0; x < 64; x++){
            state->display[y][x] = 0;
        }
    }
    printf("cleared display");
    state->draw_flag = 1;
}

static void draw_instruction(struct chip8* state, uint8_t X, uint8_t Y, uint8_t N){
    uint8_t xpos = state->registers[X] % 64;
    uint8_t ypos = state->registers[Y] % 32;

    state->registers[0xF] = 0;

    for(int row = 0; row < N; row++){
        uint8_t sprite_data = state->memory[state->index_register + row];

        uint8_t x = xpos;
        uint8_t y = ypos + row;

        if(y >= 32) break;

        for(int bit = 0; bit < 8; bit++){
            if(x >= 64) break;

            uint8_t sprite_pixel = (sprite_data & (0x80 >> bit)) != 0;

            if(sprite_pixel){
                if(state->display[y][x] == 1){
                    state->display[y][x] = 0;
                    state->registers[0xF] = 1;
                }else{
                    state->display[y][x] = 1;
                }
            }

            x++;
        }
    }
    state->draw_flag = 1;
}

static void execute_logic_arith_instruction(struct chip8* state, struct decoded_instruction instruction){
    switch(instruction.N){
        case 0:
            state->registers[instruction.X] = state->registers[instruction.Y];
            break;
        case 1:
            state->registers[instruction.X] = state->registers[instruction.X] | state->registers[instruction.Y];
            break;
        case 2:
            state->registers[instruction.X] = state->registers[instruction.X] & state->registers[instruction.Y];
            break;
        case 3:
            state->registers[instruction.X] = state->registers[instruction.X] ^ state->registers[instruction.Y];
            break;
        case 4:
            state->registers[0xF] = state->registers[instruction.X] + state->registers[instruction.Y] > 255;
            state->registers[instruction.X] = state->registers[instruction.X] + state->registers[instruction.Y];
            break;
        case 5:
            state->registers[0xF] = state->registers[instruction.X] > state->registers[instruction.Y];
            state->registers[instruction.X] = state->registers[instruction.X] - state->registers[instruction.Y];
            break;
        case 6:
            state->registers[instruction.X] = state->registers[instruction.Y];
            state->registers[0xF] = state->registers[instruction.X] & 0x1;
            state->registers[instruction.X] = state->registers[instruction.X] >> 1;
            break;
        case 7:
            state->registers[0xF] = state->registers[instruction.X] < state->registers[instruction.Y];
            state->registers[instruction.X] = state->registers[instruction.Y] - state->registers[instruction.X];
            break;
        case 8:
            state->registers[instruction.X] = state->registers[instruction.Y];
            state->registers[0xF] = (state->registers[instruction.X] >> 7) & 0x1;
            state->registers[instruction.X] = state->registers[instruction.X] << 1;
            break;
    }
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
        case 0x0:
            if(instruction.NNN == 0x0E0){
                clear_display(state);
            }else if(instruction.NNN == 0x0EE){
                if(state->stack_pointer == 0){
                    fprintf(stderr, "Trying to return from subroutine without calling one first");
                    exit(1);
                }
                state->pc = state->stack[state->stack_pointer--];
            }
            break;
        case 0x1:
            printf("jumped to %04X\n", instruction.NNN);
            state->pc = instruction.NNN;
            break;
        case 0x2:
            if(state->stack_pointer == 15) {
                fprintf(stderr, "StackOverflowException");
                exit(1);
            }
            state->stack[++state->stack_pointer] = state->pc;
            state->pc = instruction.NNN;
            break;
        case 0x3:
            if(state->registers[instruction.X] == instruction.NN){
                state->pc+=2;
            }
            break;
        case 0x4:
            if(state->registers[instruction.X] != instruction.NN){
                state->pc+=2;
            }
            break;
        case 0x5:
            if(state->registers[instruction.X] == state->registers[instruction.Y]){
                state->pc+=2;
            }
            break;
        case 0x6:
            state->registers[instruction.X] = instruction.NN;
            break;
        case 0x7:
            state->registers[instruction.X] += instruction.NN;
            break;
        case 0x8:
            execute_logic_arith_instruction(state, instruction);
            break;
        case 0x9:
            if(state->registers[instruction.X] != state->registers[instruction.Y]){
                state->pc+=2;
            }
            break;
        case 0xA:
            state->index_register = instruction.NNN;
            break;
        case 0xB:
            state->pc = instruction.NNN + state->registers[0];
            break;
        case 0xC:
            state->registers[instruction.X] = rand() & instruction.NN;
            break;
        case 0xD:
            draw_instruction(state, instruction.X, instruction.Y, instruction.N);
            break;
        case 0xE:
            /*if(instruction.NN == 0x9E){
                
            }*/
            printf("todo input");
            break;
        case 0xF:
            if(instruction.NN == 0x07){
                state->registers[instruction.X] = state->delay_timer;
            }else if(instruction.NN == 0x15){
                state->delay_timer = state->registers[instruction.X];
            }else if(instruction.NN == 0x18){
                state->delay_timer = state->registers[instruction.X];
            }else if(instruction.NN == 0x1E){
                state->index_register += state->registers[instruction.X];
                state->registers[0xF] = state->index_register >= 0x1000;
            }else if(instruction.NN == 0x33){
                state->memory[state->index_register] = state->registers[instruction.X]/100;
                state->memory[state->index_register+1] = state->registers[instruction.X]/10 % 10;
                state->memory[state->index_register+2] = state->registers[instruction.X] % 10;

            }else if(instruction.NN == 0x55){
                for(int i = 0; i <= instruction.X; i++){
                    state->memory[state->index_register + i] = state->registers[i];
                }
            }else if(instruction.NN == 0x65){
                for(int i = 0; i <= instruction.X; i++){
                    state->registers[i] = state->memory[state->index_register + i];
                }
            }
            break;

    }
}
