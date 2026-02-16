#ifndef ROM_LOADER_H
#define ROM_LOADER_H

#include "../chip8.h"

int load_rom(struct chip8 state, char* path);

#endif // ROM_LOADER_H
