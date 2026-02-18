#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdbool.h>

int create_window();

int update_display(struct chip8* state);

void destroy_display();

bool is_key_down(uint8_t key);
#endif // DISPLAY_H
