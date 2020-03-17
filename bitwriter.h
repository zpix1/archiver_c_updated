#pragma once

#include <stdio.h>
#include <stdlib.h>

#define BW_BUFFER_SIZE 1000

typedef struct BitWriter {
    FILE* f;
    char* buffer;
    char current_byte;
    int byte_offset;
    int buffer_offset;
} BitWriter;

// OOP Simulator

// f should be opened in binary writing mode
BitWriter* BitWriter__new(FILE* f);

void BitWriter__write_bit(BitWriter* bw, char bit);

void BitWriter__write_buffer(BitWriter* bw);

void BitWriter__destroy(BitWriter* bw);
