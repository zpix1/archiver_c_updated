all:
	gcc -g -Wall pq.c coder.c packer.c bitreader.c bitwriter.c cli.c -o huffman

test:
	gcc -g -Wall pq.c coder.c packer.c bitreader.c bitwriter.c test.c -o test.out

do_tester:
	gcc -g -Wall pq.c coder.c packer.c bitreader.c bitwriter.c do_tester.c -o do_tester.out -O3 -DNDEBUG   -Werror -Wall -Wextra -Waddress -Warray-bounds -Wbuiltin-macro-redefined -Wswitch -Wunreachable-code -pedantic -pedantic-errors -std=c99 -MD