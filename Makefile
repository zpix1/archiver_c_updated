all:
	gcc -g -O2 -Wall pq.c coder.c bitreader.c bitwriter.c test.c -o test.out
	./test.out

clean:
	rm -r *.dSYM test text.bin