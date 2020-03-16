#include <string.h>

#include "coder.h"
#include "pq.h"
#include "bitwriter.h"

int reversed_cmp_freq_node(const void* a, const void* b) {
    return -(((FreqNode*)a)->freq - ((FreqNode*)b)->freq);
}

int min(int a, int b) {
    return a < b ? a : b;
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
    while ((read = fread(buffer, sizeof(char), min(to_read, C_BUFFER_SIZE), infile))) {
        for (size_t i = 0; i < read; i++) {
            arr[buffer[i]]->freq++;
        }
        to_read -= read;
    }
    // to_read should be size but if not 
    // In case size is bigger than actual file size
    fseek(infile, (to_read - size), SEEK_CUR);

    // To count different nodes
    int counter = 0;

    priority_queue* pq = create_pq(sizeof(FreqNode*), reversed_cmp_freq_node);

    for (size_t i = 0; i < BYTE_COUNT; i++) {
        if (arr[i]->freq != 0) {
            counter++;
            pq_add(pq, &arr[i]);
        } else {
            free(arr[i]);
        }
    }

    FreqNode *ans;
    // Special case of one element
    if (counter == 1) {
        ans = *((FreqNode**)pq_pop(pq));
    } else {
        while (pq->n_items != 1) {
            FreqNode* new_node = malloc(sizeof(FreqNode));
            new_node->zero_node = *((FreqNode**)pq_pop(pq));
            new_node->one_node = *((FreqNode**)pq_pop(pq));

            PRINT_FN(new_node->zero_node);
            PRINT_FN(new_node->one_node);
            
            new_node->freq = new_node->one_node->freq + new_node->zero_node->freq;
            new_node->c = 0;
            pq_add(pq, &new_node);
            PRINT_FN(new_node);
            counter--;
        }
        ans = *((FreqNode**)pq_pop(pq));
    }

    free(arr);
    free(buffer);
    delete_pq(pq);

    return ans;
}

void free_tree(FreqNode* head) {
    if (head != NULL) {
        free_tree(head->one_node);
        free_tree(head->zero_node);
        free(head);
    }
}

void fill_code_table(CharCode* code_table, FreqNode* node, CharCode current_code) {
    PRINT_FN(node);
    if (node->one_node != NULL) {
        CharCode c;
        c.code_length = current_code.code_length + 1;
        c.code =  (current_code.code << 1) + 1;
        fill_code_table(code_table, node->one_node, c);
    }
    if (node->zero_node != NULL) {
        CharCode c;
        c.code_length = current_code.code_length + 1;
        c.code =  (current_code.code << 1);
        fill_code_table(code_table, node->zero_node, c);
    }
    if (node->one_node == NULL) {
        code_table[node->c] = current_code;
    }
}

void encode_data(FreqNode* freq_tree, FILE* infile, size_t size, FILE* outfile) {
    CharCode* code_table = malloc(sizeof(CharCode) * BYTE_COUNT);
    for (int i = 0; i < BYTE_COUNT; i++) {
        code_table[i].code_length = 0;
        code_table[i].code = 0;
    }
    CharCode c = {.code_length = 0, .code = 0};
    fill_code_table(code_table, freq_tree, c);
    for (int i = 0; i < BYTE_COUNT; i++) {
        if (code_table[i].code_length != 0)
        printf("%c: " BYTE_TO_BINARY_PATTERN "; %d\n", i, BYTE_TO_BINARY(code_table[i].code), code_table[i].code_length);
    }
    unsigned char* buffer = malloc(sizeof(char) * C_BUFFER_SIZE);
    int to_read = size;
    int read = 0;

    BitWriter* bw = BitWriter__new(outfile);
    while ((read = fread(buffer, sizeof(char), min(to_read, C_BUFFER_SIZE), infile))) {
        for (size_t byte_i = 0; byte_i < read; byte_i++) {
            unsigned char byte = buffer[byte_i];
            for (size_t bit_i = 0; bit_i < code_table[byte].code_length; bit_i++) {
                BitWriter__write_bit(bw, (code_table[byte].code >> (code_table[byte].code_length - bit_i - 1)) & 1);
            }
        }
        to_read -= read;
    }
    BitWriter__write_buffer(bw);
    BitWriter__destroy(bw);
    free(buffer);
    free(code_table);
}