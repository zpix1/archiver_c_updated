#include "bitreader.h"

BitReader* BitReader__new(FILE* f) {
    BitReader* br = malloc(sizeof(BitReader));
    br->f = f;
    br->buffer = malloc(sizeof(char) * BR_BUFFER_SIZE);
    br->current_byte = 0;
    br->byte_offset = 0;
    br->buffer_offset = BR_BUFFER_SIZE;
    br->max_buffer_size = BR_BUFFER_SIZE;
    return br;
}

char BitReader__read_bit(BitReader* this) {
    if (this->buffer_offset == this->max_buffer_size) {
        this->max_buffer_size =
            fread(this->buffer, sizeof(char), BR_BUFFER_SIZE, this->f);
        this->buffer_offset = 0;
    }
    if (this->max_buffer_size == 0) {
        return -1;
    }
    char ans =
        (this->buffer[this->buffer_offset] & (1 << (7 - this->byte_offset))) >>
        (7 - this->byte_offset);

    this->byte_offset++;

    if (this->byte_offset == 8) {
        this->byte_offset = 0;
        this->buffer_offset++;
    }
    return ans;
}

void BitReader__next_byte(BitReader* this) {
    while (this->byte_offset != 0) {
        BitReader__read_bit(this);
    }
}

int BitReader__read_byte(unsigned char* byte_ptr, BitReader* this) {
    unsigned char ans = 0;
    for (int i = 0; i < 8; i++) {
        ans |= BitReader__read_bit(this) << (7 - i);
    }
    *byte_ptr = ans;
    return 1;
}

void BitReader__destroy(BitReader* this) {
    fseek(this->f, -this->max_buffer_size + this->buffer_offset + 1, SEEK_CUR);
    // fprintf(stdout, "pwd=%d\nfteel=%d\n", -this->max_buffer_size + this->byte_offset, ftell(this->f));
    free(this->buffer);
    free(this);
}
