#include <stdlib.h>
#include <stdio.h>

#define BYTE_COUNT 256
#define C_BUFFER_SIZE 1000

typedef struct FreqNode {
    int freq;
    unsigned char c;

    struct FreqNode* zero_node;
    struct FreqNode* one_node;
} FreqNode;

// Generate freq_tree and return the caret to the original state
FreqNode* generate_code_tree(FILE* infile, size_t size);

// Encode and write size bytes from infile to outfile and don't change caret state
void encode_data(FreqNode* freq_tree, FILE* infile, size_t size, FILE* outfile);