# CC            = gcc
# CFLAGS        = -O4 -Wall -I/usr/local/include
# DEST          = /usr/local/bin
# LDFLAGS       = -L/usr/local/lib
# LIBS          = -lhoge -lm
# OBJS          = main.o sub.o
# PROGRAM       = myprogram

# all:            $(PROGRAM)

# $(PROGRAM):     $(OBJS)
#                 $(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)

all:
	make server client

server: server.c
	# gcc -static server.c libcoap.c SHA256.c -o server.o
	gcc server.c libcoap.c SHA256.c -o server.o

client: client.c
	# gcc -static client.c libcoap.c SHA256.c -o client.o
	gcc client.c libcoap.c SHA256.c -o client.o

test: test.c
	gcc test.c -o test.o

check: check.c
	gcc check.c -o check.o