#include <stdio.h>
#include <stdlib.h>
#include "utils/rom_loader.h"
#include "chip8.h"
#include "display.h"

int main(int argc, char** argv){
    if(argc != 2){
        printf("Help: ./chip8 <rom-path>\n");
        return 1;
    }
    struct chip8 state = {0};
    if(chip8_init(&state, argv[1]) && create_window()){
        chip8_start_loop(&state);
    }
    destroy_display();
}
