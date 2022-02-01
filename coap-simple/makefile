server: server.c
	gcc -static server.c libcoap.c SHA256.c -o server.o

client: client.c
	gcc -static client.c libcoap.c SHA256.c -o client.o

test: test.c
	gcc test.c -o test.o

check: check.c
	gcc check.c -o check.o