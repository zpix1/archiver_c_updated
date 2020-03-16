all:
	gcc -g -Wall pq.c coder.c bitreader.c bitwriter.c test.c -o test.out

clean:
	rm -r *.dSYM test text.bin