#include "packer.h"
#include "coder.h"

#include <sys/stat.h>
#include <sys/types.h>

unsigned long fsize(FILE* file)
{
    unsigned long orig = (unsigned long)ftell(file);
    fseek(file, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(file);
    fseek(file, orig, SEEK_SET);
    return len;
}

int pack_file(FILE* infile, FILE* outfile) {
    // We don't work with files more than 2GB
    int infile_size = (int)fsize(infile); // dont forget to add -3 here bc of tester
    FreqNode* tree = generate_code_tree(infile, infile_size);
    
    fwrite(&infile_size, sizeof(int), 1, outfile);
    write_tree(tree, outfile);
    encode_data(tree, infile, infile_size, outfile);

    free_tree(tree);
    return 0;
}

int unpack_file(FILE* infile, FILE* outfile) {
    int infile_size;
    fread(&infile_size, sizeof(int), 1, infile);
    
    FreqNode* tree = read_tree(infile);
    decode_data(tree, infile, infile_size, outfile);

    free_tree(tree);
    return 0;
}