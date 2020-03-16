#include "coder.h"
#include "pq.h"

int reversed_cmp_freq_node(const void* a, const void* b) {
    return -(((FreqNode*)a)->freq - ((FreqNode*)b)->freq);
}

int max(int a, int b) {
    return a > b ? a : b;
}

FreqNode* generate_code_tree(FILE* infile, size_t size) {
    FreqNode** arr = malloc(BYTE_COUNT * sizeof(FreqNode*));
    
    for (size_t i = 0; i < BYTE_COUNT; i++) {
        arr[i] = malloc(sizeof(FreqNode));
        arr[i]->freq = 0;
        arr[i]->c = (unsigned char)i;
        arr[i]->zero_node = NULL;
        arr[i]->one_node = NULL;
    }

    size_t to_read = size;
    size_t read = 0;
    unsigned char* buffer = malloc(C_BUFFER_SIZE * sizeof(char));
    while ((read = fread(buffer, sizeof(char), max(to_read, C_BUFFER_SIZE), infile))) {
        for (size_t i = 0; i < read; i++) {
            arr[buffer[i]]->freq++;
        }
        to_read -= read;
    }
    fseek(infile, -size, SEEK_CUR);

    int different_char_count = 0;

    priority_queue* pq = create_pq(sizeof(FreqNode*), reversed_cmp_freq_node);

    for (size_t i = 0; i < BYTE_COUNT; i++) {
        if (arr[i]->freq != 0) {
            different_char_count++;
            pq_add(pq, arr[i]);
        } else {
            free(arr[i]);
        }
    }

    free(arr);
    free(buffer);

    return pq_pop(pq);
}

void encode_data(FreqNode* freq_tree, FILE* infile, size_t size, FILE* outfile) {

}