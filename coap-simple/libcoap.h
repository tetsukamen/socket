#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFF_SIZE 128
#define SECRET "aaaaaaaaa"
#define TICKET_SIZE 10
#define IP_SIZE 15
#define COAP_BUF_MAX_SIZE 128
#define COAP_HEADER_SIZE 4
#define COAP_TOKEN_SIZE 1
#define OPTION_LENGTH 1

struct CoapPacket
{
  /* data */
};

struct CoapOption
{
  int delta;
  int length;
  uint8_t value;
};

struct Message
{
  uint8_t payload[BUFF_SIZE] = {0};
  char ip[IP_SIZE];
  int port;
  char ticket[TICKET_SIZE];
  CoapOption options[OPTION_LENGTH];
};

int listenCoapPacketStart(char *ip, int port);
void listenCoapPacketEnd(int sock);
int sendCoapPacket(int sock, char *payload, int payload_size, char *dist_ip,
                   int dist_port, char *ticket);
Message recvCoapPacket(int sock);
char *SHA(char *ip, char *secret);
char *generateTicket(char *ip);
int validateTicket(char *ticket, char *ip);