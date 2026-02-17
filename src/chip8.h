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
};

void chip8_init(struct chip8* state, char* path);


#endif // CHIP_8
