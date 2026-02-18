#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

struct chip8 {
    uint8_t memory[4096];
    uint16_t pc;

    uint16_t stack[16];
    uint8_t stack_pointer;

    uint8_t delay_timer;
    uint8_t sound_timer;

    uint16_t index_register;
    uint8_t registers[16];

    uint8_t display[32][64];
    uint8_t draw_flag;
};

struct decoded_instruction {
    uint16_t instruction;
    uint8_t opcode;
    uint8_t X;
    uint8_t Y;

    uint8_t N;
    uint8_t NN;
    uint16_t NNN;
};

int chip8_init(struct chip8* state, char* path);

void chip8_start_loop(struct chip8* state);

uint16_t chip8_fetch_instruction(struct chip8* state);

struct decoded_instruction chip8_decode_instruction(uint16_t instruction);

#endif // CHIP_8
