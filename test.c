#include <assert.h>
#include <stdio.h>

#include "bitwriter.h"
#include "bitreader.h"
#include "coder.h"

#define BWBR_TEST_SIZE 1000000

void test_bw_br() {
    printf("[TEST] BitWriter test\n");

    FILE* f = fopen("text.bin", "wb");
    BitWriter* bw = BitWriter__new(f);
    int a[] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1};
    
    printf("[TEST] BitWriter test start\n");
    for (int i = 0; i < BWBR_TEST_SIZE; i++) {
        BitWriter__write_bit(bw, a[i % (sizeof(a)/sizeof(int))]);
    }
    printf("[TEST] BitWriter test end\n");

    BitWriter__write_buffer(bw);
    BitWriter__destroy(bw);
    fclose(f);

    printf("[TEST] BitReader test\n");

    FILE* f1 = fopen("text.bin", "rb");
    BitReader* br = BitReader__new(f1);
    char bit = 0;
    int i = 0;
    while ((bit = BitReader__read_bit(br)) != -1) {
        if (bit != a[i % (sizeof(a)/sizeof(int))]) {
            printf("bit reader or bitwriter error\n");
            break;
        }
        i++;
        if (i == BWBR_TEST_SIZE) {
            break;
        }
    }

    BitReader__destroy(br);
    fclose(f1);
}

int main() {
    FILE* f = fopen("infile.txt", "rb");
    FILE* of = fopen("outfile.txt", "wb");

    const int infile_size = 200;

    printf("[TEST] Generating code tree\n");
    FreqNode* head = generate_code_tree(f, infile_size);
    printf("[TEST] Encoding data\n");
    encode_data(head, f, infile_size, of);
    printf("%d\n", head->freq);
    free_tree(head);
    return 0;
}