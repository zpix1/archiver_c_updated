#pragma once

#include <stdlib.h>
#include <stdio.h>

unsigned long fsize(FILE* file);

// Pack files to output_filename using huffman coding according to this file structure:
// 4b (uint) - n - amount of files
// desc of n filenames:
//      4b (uint) - filename_s - filename size
//      4b (uint) - file_s - file size
//      filename_s b - filename
// desc of n files:
//      k bytes - decoding tree
//      file_s bytes - encoded data
int pack_files(char** filenames, size_t nfilenames, char* outfilename);

// Unpack files from file previously created with pack_files to outdir
int unpack_files(char* infilename, char* outdir);

// Compress and write infile to given position of outfile
int pack_file(FILE* infile, FILE* outfile, int infile_size);

int unpack_file(FILE* infile, FILE* outfile);