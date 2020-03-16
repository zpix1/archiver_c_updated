#include <string.h>

#include "coder.h"
#include "pq.h"
#include "bitwriter.h"
#include "bitreader.h"

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

    heap_t *h = calloc(1, sizeof(heap_t));

    for (size_t i = 0; i < BYTE_COUNT; i++) {
        if (arr[i]->freq != 0) {
            counter++;
            push(h, arr[i]->freq, arr[i]);
        } else {
            free(arr[i]);
        }
    }

    FreqNode *ans;
    // Special case of one element
    if (counter == 1) {
        ans = pop(h);
    } else {
        while (h->len != 1) {
            FreqNode* new_node = malloc(sizeof(FreqNode));
            new_node->zero_node = pop(h);
            new_node->one_node = pop(h);
            
            new_node->freq = new_node->one_node->freq + new_node->zero_node->freq;
            new_node->c = 0;
            push(h, new_node->freq, new_node);
            counter--;
        }
        ans = pop(h);
    }

    free(arr);
    free(buffer);
    free(h->nodes);
    free(h);

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

    // printf("[TEST] Code table:\n");
    // for (int i = 0; i < BYTE_COUNT; i++) {
    //     if (code_table[i].code_length != 0)
    //     printf("char=%c: code=" BYTE_TO_BINARY_PATTERN "; code_len=%d\n", i, BYTE_TO_BINARY(code_table[i].code), code_table[i].code_length);
    // }

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

void decode_data(FreqNode* freq_tree, FILE* infile, size_t size, FILE* outfile) {
    unsigned char* buffer = malloc(sizeof(char) * C_BUFFER_SIZE);
    int to_write = size;
    int buffer_i = 0;
    int counter = 0;

    FreqNode* current_node = freq_tree;
    if (current_node->one_node == NULL) {
        memset(buffer, current_node->c, C_BUFFER_SIZE);
        while (to_write > 0) {
            fwrite(buffer, sizeof(char), min(to_write, C_BUFFER_SIZE), outfile);
            to_write -= C_BUFFER_SIZE;
        }
    } else {
        BitReader* br = BitReader__new(infile);
        while (counter < size) {
            char bit = BitReader__read_bit(br);
            if (bit == 0) {
                current_node = current_node->zero_node;
            } else if (bit == 1) {
                current_node = current_node->one_node;
            }
            if (bit == -1) {
                break;
            }
            if (current_node->one_node == NULL) {
                buffer[buffer_i] = current_node->c;
                current_node = freq_tree;
                counter++;
                buffer_i++;
                if (buffer_i == min(to_write, C_BUFFER_SIZE)) {
                    to_write -= buffer_i;
                    fwrite(buffer, sizeof(char), buffer_i, outfile);
                    buffer_i = 0;
                }
            }
        }

        if (buffer_i != 0) {
            fwrite(buffer, sizeof(char), min(buffer_i, to_write), outfile);
        }
        BitReader__destroy(br);
    }
    free(buffer);
}

void write_tree(FreqNode* freq_tree, FILE* infile) {
    int zero = 0;

    if (freq_tree == NULL) {
        fwrite(&zero, sizeof(char), 1, infile);
        return;
    }

    // 0 - NULL, 1 - LEAF, 2 - NODE
    char flag = (freq_tree->one_node == NULL ? 1 : 2);

    fwrite(&(flag), sizeof(char), 1, infile);

    // If it is a leaf, save the value
    if (flag == 1) {
        fwrite(&(freq_tree->c), sizeof(char), 1, infile);
    }

    write_tree(freq_tree->zero_node, infile);
    write_tree(freq_tree->one_node, infile);
}

FreqNode* read_tree(FILE* fp) {
    int value = 0;
    char flag;

    fread(&flag, sizeof(char), 1, fp);

    if (flag == 0) {
        return NULL;
    } else if (flag == 1) {
        fread(&value, sizeof(char), 1, fp);
    }

    FreqNode *root = malloc(sizeof(FreqNode));
    root->freq = 0;
    root->c = value;

    root->zero_node = read_tree(fp);
    root->one_node = read_tree(fp);
    return root;
}