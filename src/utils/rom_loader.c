#include <stdio.h>

#include "../chip8.h"

static int compute_file_len(FILE* file)
{
    fseek(file, 0L, SEEK_END);
    int romLen = ftell(file);
    fseek(file, 0L, SEEK_SET);
    return romLen;
}

int load_rom(struct chip8 state, char* path)
{
    printf("Loading rom %s...\n", path);
    FILE *f = fopen(path, "rb");
    if(f == NULL){
        printf("Failed to open rom\n");
        return 0;
    }

    size_t fileLen = compute_file_len(f);

    if(fread(state.memory + 200, 1, fileLen, f) != fileLen) {
        printf("Failed to read rom content\n");
        return 0;
    }
    fclose(f);

    printf("Sucessfully loaded %ld bytes from rom\n", fileLen);
    return 1;
}
