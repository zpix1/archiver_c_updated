#include <assert.h>
#include <stdio.h>

#include "bitwriter.h"
#include "bitreader.h"

#define BWBR_TEST_SIZE 1000000

int main() {
    printf("[TEST] BitWriter test\n");

    FILE* f = fopen("text.bin", "wb");
    BitWriter* bw = BitWriter__new(f);
    int a[] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1};
    for (int i = 0; i < BWBR_TEST_SIZE; i++) {
        BitWriter__write_bit(bw, a[i % (sizeof(a)/sizeof(int))]);
    }

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