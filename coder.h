#include <stdlib.h>
#include <stdio.h>

#define BYTE_COUNT 256
#define C_BUFFER_SIZE 1000

#define PRINT_FN(FN) printf("%20s: freq=%5d, c='%c',  zero=%p, one=%p (%p)\n", #FN,  FN->freq, FN->c, FN->zero_node, FN->one_node, FN);
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

typedef struct FreqNode {
    int freq;
    unsigned char c;

    struct FreqNode* zero_node;
    struct FreqNode* one_node;
} FreqNode;

typedef struct CharCode {
    char code_length;
    unsigned char code;
} CharCode;

void free_tree(FreqNode* head);

// Generate freq_tree and return the caret to the original state
FreqNode* generate_code_tree(FILE* infile, size_t size);

// Encode and write size bytes from infile to outfile and don't change caret state
void encode_data(FreqNode* freq_tree, FILE* infile, size_t size, FILE* outfile);