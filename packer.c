#include "packer.h"
#include "coder.h"
#include "bitwriter.h"
#include "bitreader.h"

#include <sys/stat.h>
#include <sys/types.h>

unsigned long fsize(FILE* file) {
    unsigned long orig = (unsigned long)ftell(file);
    fseek(file, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(file);
    fseek(file, orig, SEEK_SET);
    return len;
}

int pack_file(FILE* infile, FILE* outfile, unsigned int infile_size) {
    FreqNode* tree = generate_code_tree(infile, infile_size);

    fwrite(&infile_size, sizeof(unsigned int), 1, outfile);

    BitWriter* bw = BitWriter__new(outfile);
    write_tree(tree, bw);
    BitWriter__write_buffer(bw);
    BitWriter__destroy(bw);
    encode_data(tree, infile, infile_size, outfile);

    free_tree(tree);

    return 0;
}

int unpack_file(FILE* infile, FILE* outfile) {
    unsigned int infile_size;
    fread(&infile_size, sizeof(unsigned int), 1, infile);
    
    BitReader* br = BitReader__new(infile);
    FreqNode* tree = read_tree(br);
    BitReader__destroy(br);
    decode_data(tree, infile, infile_size, outfile);

    free_tree(tree);

    return 0;
}