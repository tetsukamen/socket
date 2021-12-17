#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFF_SIZE 1024
#define SECRET "aaaaaaaaa"
#define TICKET_SIZE 10
#define DATA_SIZE 20
#define IP_SIZE 15

struct CoapPacket {
  /* data */
};

struct Message {
  char payload[BUFF_SIZE];
  char ip[IP_SIZE];
  int port;
  char ticket[TICKET_SIZE];
};

int listenCoapPacketStart(char *ip, int port);
void listenCoapPacketEnd(int sock);
int sendCoapPacket(char *payload, char *ip, int port, char *ticket);
Message recvCoapPacket(int sock);
char *SHA(char *ip, char *secret);
char *generateTicket(char *ip);
int validateTicket(char *ticket, char *ip);