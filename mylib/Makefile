test.out: test.c pylist.c
	gcc -c test.c -o test.o
	gcc -c pylist.c -o pylist.o
	gcc test.o pylist.o -o test.out

test2.out: test2.o pylist.o
	gcc test2.o pylist.o -o test2.out

test2.o: test2.c
	gcc -c test2.c -o test2.o

pylist.o: pylist.c
	gcc -c pylist.c -o pylist.o

