#pragma once

#include <stdio.h>
#include <stdlib.h>

#define BR_BUFFER_SIZE 1000

typedef struct BitReader {
    FILE* f;
    char* buffer;
    char current_byte;
    int byte_offset;
    int buffer_offset;
    int max_buffer_size;
} BitReader;

// f should be opened in binary reading mode
BitReader* BitReader__new(FILE* f);

char BitReader__read_bit(BitReader* br);

int BitReader__read_byte(unsigned char* byte, BitReader* this);

void BitReader__next_byte(BitReader* this);

void BitReader__destroy(BitReader* br);
