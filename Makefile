server.out: server.o mylib/pylist.o
	gcc server.o mylib/pylist.o -lpthread -o server.out

server.o: server.c
	gcc -c server.c -o server.o

client.out: client.c
	gcc client.c -lpthread -o client.out

