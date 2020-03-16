#include "packer.h"
#include "coder.h"

#include <sys/stat.h>
#include <sys/types.h>

unsigned long fsize(FILE* file)
{
    fseek(file, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(file);
    fseek(file, 0, SEEK_SET);
    return len;
}

int pack_file(FILE* infile, FILE* outfile) {
    // We don't work with files more than 2GB
    int infile_size = (int)fsize(infile);
    
    fprintf(stderr, "d=%d\n", infile_size);
    FreqNode* tree = generate_code_tree(infile, infile_size);
    
    fwrite(&infile_size, sizeof(int), 1, outfile);
    write_tree(tree, outfile);
    encode_data(tree, infile, infile_size, outfile);

    return 0;
}

int unpack_file(FILE* infile, FILE* outfile) {
    int infile_size;
    fread(&infile_size, sizeof(int), 1, infile);
    fprintf(stderr, "d=%d\n", infile_size);
    FreqNode* tree = read_tree(infile);
    decode_data(tree, infile, infile_size, outfile);

    return 0;
}