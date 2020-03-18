all:
	gcc -g -Wall pq.c coder.c packer.c bitreader.c bitwriter.c cli.c -o test_dir/huffman

test:
	gcc -g -Wall pq.c coder.c packer.c bitreader.c bitwriter.c test.c -o test.out

clean:
	rm -r *.dSYM test.out text.bin

do_tester:
	gcc -g -Wall pq.c coder.c packer.c bitreader.c bitwriter.c do_tester.c -o do_tester.out