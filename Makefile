all:
	gcc -g -Wall pq.c coder.c bitreader.c bitwriter.c test.c -o test.out

clean:
	rm -r *.dSYM test text.bin

do_tester:
	gcc -g -Wall pq.c coder.c packer.c bitreader.c bitwriter.c do_tester.c -o do_tester.out