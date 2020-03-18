#include <stdio.h>
#include <string.h>

#include "packer.h"

void usage() {
    printf("usage:\thuffman [c]ompress [-r, --recursive] <archive_name> [<file/directory_names>...]\n");
    printf("\thuffman [d]ecompress <archive_name> [<output_dir_name>]\n");
}

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "error: not enough aruments, check usage\n");
        usage();
        return 1;
    }
    if (!strcmp(argv[1], "c") || !strcmp(argv[1], "compress")) {
        int is_recursive = 0;
        
        size_t nfilenames = argc - 3;
        char* outfilename = argv[1];
        printf("compressing to %s\n", outfilename);

        if (!strcmp(argv[2], "-r") || !strcmp(argv[2], "--recursive")) {
            is_recursive = 1;
            nfilenames--;
            printf("using recursion for paths\n");
        }
        char** filenames = NULL;
        if (!is_recursive) {
            for (int i = argc - nfilenames; i < argc; i++) {
                printf("argc = %s\n", argv[i]);
                if (isdir(argv[i])) {
                    fprintf(stderr, "error: %s is dir, but recursive mode is not enabled, aborting\n");
                    return 1;
                }
            }
            filenames = argv + argc - nfilenames;
        } else {
            
        }
        
    } else if (!strcmp(argv[1], "d") || !strcmp(argv[1], "decompress")) {

    } else {
        printf("error: wrong mode, check usage\n");
        usage();
    }
    return 0;
}