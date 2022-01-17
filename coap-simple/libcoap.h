#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "SHA256.h"

#define BUFF_SIZE 128
#define SECRET "abcdefghij"
#define TICKET_SIZE 10
#define IP_SIZE 15
#define COAP_BUF_MAX_SIZE 128
#define COAP_HEADER_SIZE 4
#define COAP_TOKEN_SIZE 1
#define OPTION_LENGTH 2

struct CoapPacket {
  /* data */
};

struct CoapOption {
  int delta;
  int length;
  uint64_t value;
};

struct Message {
  int version;
  int type;
  int tokenLength;
  int code;
  uint8_t messageIdUpper;
  uint8_t messageIdLower;
  uint8_t token;
  uint8_t payload[BUFF_SIZE] = {0};
  char ip[IP_SIZE];
  int port;
  char ticket[TICKET_SIZE];
  CoapOption options[OPTION_LENGTH];
};

int listenCoapPacketStart(char *ip, int port);
void listenCoapPacketEnd(int sock);
int sendCoapPacket(int sock, char *payload, int payload_size, char *dist_ip,
                   int dist_port, uint64_t ticket = 0);
Message recvCoapPacket(int sock);
uint64_t SHA(char *ip, char *secret);
uint64_t generateTicket(char *ip);
int validateTicket(uint64_t ticket, char *ip);