#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000

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

inline void BitWriter__write_bit(BitWriter* bw, char* bit);

void BitWriter__write_buffer(BitWriter* bw);

void BitWriter__destroy(BitWriter* bw);
