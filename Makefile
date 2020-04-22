server.out: server.o mylib/pylist.o
	gcc server.o mylib/pylist.o -lpthread -o server.out

server.o: server.c
	gcc -c server.c -o server.o

client.out: client.o graphics/cui.o
	gcc client.o graphics/cui.o -lpthread -o client.out

client.o: client.c
	gcc -c client.c -o client.o

graphics/cui.o: graphics/cui.c
	gcc -c graphics/cui.c -o graphics/cui.o
