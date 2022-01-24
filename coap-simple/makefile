server: server.c
	gcc server.c libcoap.c SHA256.c -o server.o

client: client.c
	gcc client.c libcoap.c SHA256.c -o client.o

test: test.c
	gcc test.c -o test.o

check: check.c
	gcc check.c -o check.o