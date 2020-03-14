all:
	gcc -O2 -Wall bitreader.c bitwriter.c test.c -o test.out
	./test

clean:
	rm -r *.dSYM test text.bin