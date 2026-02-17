#include "chip8.h"
#include "utils/rom_loader.h"

#include <stdio.h>

void chip8_init(struct chip8* state, char* path)
{
    load_rom(state, path);
    for(size_t i = 0; i < 4096; i++){
        if(state->memory[i] != 0){
            printf("%02X\n", state->memory[i]);
        }
    }
}
