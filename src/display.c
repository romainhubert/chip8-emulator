#include <SDL2/SDL.h>
#include <stdio.h>

static SDL_Window* window = NULL;

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

    return 1;
}

int update_display(){
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            return 0;
    }
    SDL_Delay(16);
    return 1;
}

void destroy_display(){
    SDL_DestroyWindow(window);
    SDL_Quit();
}
