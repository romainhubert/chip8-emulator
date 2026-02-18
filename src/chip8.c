#define _POSIX_C_SOURCE 199309L

#include "chip8.h"
#include "utils/rom_loader.h"
#include "display.h"

#include <time.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#define TIMER_UPDATE_PER_SECOND 60
#define INSTRUCTIONS_PER_SECOND 1

int chip8_init(struct chip8* state, char* path)
{
    state->pc = 0x200;
    state->draw_flag = 0;
    return load_rom(state, path);
}

static double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

static void update_timers(struct chip8* state){
    if(state->delay_timer != 0){
        state->delay_timer--;
    }
    if(state->sound_timer != 0){
        state->sound_timer--;
    }
}

void draw_test_pattern(struct chip8* state) {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            state->display[y][x] = y == 0 || y == 31 || x == 0 || x == 63;
        }
    }
    state->draw_flag = 1;
}

void chip8_start_loop(struct chip8* state){
    double last_timer = get_time();
    double last_instruction_time = get_time();
    int running = 1;
    while(running)
    {
        double now = get_time();

        if(now - last_timer >= 1.0/TIMER_UPDATE_PER_SECOND){
            update_timers(state);
            last_timer = get_time();
            draw_test_pattern(state);
            running = update_display(state);
        }

        if(now - last_instruction_time >= 1.0/INSTRUCTIONS_PER_SECOND){
            uint16_t instruction = chip8_fetch_instruction(state);
            chip8_execute_instruction(state, instruction);
            last_instruction_time = get_time();
        }
    }
}

uint16_t chip8_fetch_instruction(struct chip8* state){
    uint16_t instruction = state->memory[state->pc++];
    return (instruction << 8) | state->memory[state->pc++];
}


void chip8_execute_instruction(struct chip8* state, uint16_t instruction){
    uint8_t opcode = (instruction & 0xF000)>>12;
    uint8_t X = (instruction & 0xF00)>>8;
    uint8_t Y = (instruction & 0xF0)>>4;

    uint8_t N = instruction & 0xF;
    uint8_t NN = instruction & 0xFF;
    uint16_t NNN = instruction & 0xFFF;
    printf("%04X\n", instruction);
    printf("opcode=%04X\n", opcode);
    printf("X=%04X\n", X);
    printf("Y=%04X\n", Y);
    printf("N=%04X\n", N);
    printf("NN=%04X\n", NN);
    printf("NNN=%04X\n", NNN);

    state->memory[0]++;
}

