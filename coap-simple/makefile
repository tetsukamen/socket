server: server.cpp
	g++ server.cpp libcoap.cpp SHA256.cpp -o server.o -std=c++14

client: client.cpp
	g++ client.cpp libcoap.cpp SHA256.cpp -o client.o -std=c++14

test: test.cpp
	g++ -w test.cpp SHA256.cpp -o test.o