#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "chip8.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;

static bool keys[322] = { 0 };

static uint8_t KEYMAP[16] = {
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v
};

int create_window(){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        fprintf(stderr, "Failed to initialize SDL2 : %s\n", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if(window == NULL){
        fprintf(stderr, "Failed to create window : %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_RenderSetLogicalSize(renderer, 64, 32);

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        64,
        32
    );
    return 1;
}

static void update_texture(struct chip8* state){
    uint32_t pixels[32 * 64];
    for(int y = 0; y < 32; y++){
        for(int x = 0; x < 64; x++){
            pixels[y * 64 + x] = state->display[y][x] ? 0xFFFFFFFF : 0x000000FF;
        }
    }
    SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    state->draw_flag = 0;
}

int update_display(struct chip8* state){
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            return 0;
        else if(e.type == SDL_KEYDOWN && e.key.keysym.sym <= 322){
            keys[e.key.keysym.sym] = true;
        } else if(e.type == SDL_KEYUP && e.key.keysym.sym <= 322){
            keys[e.key.keysym.sym] = false;
        }
    }
    if(state->draw_flag){
        update_texture(state);
    }
    return 1;
}

void destroy_display(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool is_key_down(uint8_t key){
    return keys[KEYMAP[key]];
}
