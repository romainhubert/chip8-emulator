#define _POSIX_C_SOURCE 199309L

#include "chip8.h"
#include "utils/rom_loader.h"
#include "display.h"
#include "exec_instruction.h"

#include <time.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#define TIMER_UPDATE_PER_SECOND 60
#define INSTRUCTIONS_PER_SECOND 700
#define FONT_START_ADDRESS 0x50

static const uint8_t chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

int chip8_init(struct chip8* state, char* path)
{
    state->pc = 0x200;
    state->draw_flag = 0;
    srand(time(NULL));
    for (int i = 0; i < 80; i++) {
        state->memory[FONT_START_ADDRESS + i] = chip8_fontset[i];
    }
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
            running = update_display(state);
        }

        if(now - last_instruction_time >= 1.0/INSTRUCTIONS_PER_SECOND){
            uint16_t instruction = chip8_fetch_instruction(state);
            struct decoded_instruction decoded_instruction = chip8_decode_instruction(instruction);
            execute_instruction(state, decoded_instruction);
            last_instruction_time = get_time();
        }
    }
}

uint16_t chip8_fetch_instruction(struct chip8* state){
    uint16_t instruction = state->memory[state->pc++];
    return (instruction << 8) | state->memory[state->pc++];
}

struct decoded_instruction chip8_decode_instruction(uint16_t instruction){
    uint8_t opcode = (instruction & 0xF000)>>12;
    uint8_t X = (instruction & 0xF00)>>8;
    uint8_t Y = (instruction & 0xF0)>>4;

    uint8_t N = instruction & 0xF;
    uint8_t NN = instruction & 0xFF;
    uint16_t NNN = instruction & 0xFFF;

    struct decoded_instruction di = {.instruction = instruction, .opcode = opcode, .X = X, .Y = Y, .N = N, .NN = NN, .NNN = NNN};
    return di;
}

