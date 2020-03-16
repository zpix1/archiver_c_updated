#include <stdio.h>

#include "packer.h"

int main () {
    char mode;
    scanf("%c\r\n", &mode);
    if (mode == 'c') {
        pack_file(stdin, stdout);
    } else {
        unpack_file(stdin, stdout);
    }
    return 0;
}