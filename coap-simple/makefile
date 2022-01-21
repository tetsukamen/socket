server: server.c
	gcc server.c libcoap.c SHA256.c -o server.o

client: client.c
	gcc client.c libcoap.c SHA256.c -o client.o

test: test.c
	gcc -w test.c SHA256.c -o test.o

check: check.c
	gcc -w check.c -o check.o