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

char BitReader__read_bit(BitReader* bw);

void BitReader__destroy(BitReader* bw);
