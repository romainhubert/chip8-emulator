#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

struct chip8 {
    uint8_t memory[4096];
};

void chip8_init(struct chip8 state, char* path);


#endif // CHIP_8
