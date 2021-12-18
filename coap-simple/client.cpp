#include "libcoap.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define CLIENT_IP "127.0.0.1"
#define CLIENT_PORT 8081

int main() {
  // variables declaration
  struct Message msg;
  char ticket[BUFF_SIZE];

  // start listen
  int sock = listenCoapPacketStart(CLIENT_IP, CLIENT_PORT);

  // get ticket
  errno = 0;
  sendCoapPacket(sock, "ticketRequest", SERVER_IP, SERVER_PORT, "");
  printf("send ticket request\n");
  msg = recvCoapPacket(sock);
  strcpy(ticket, msg.payload);
  printf("receve ticket: %s\n", ticket);

  // get data request 3 times
  for (int i = 0; i < 3; i++) {
    printf("send request\n");
    sendCoapPacket(sock, ticket, SERVER_IP, SERVER_PORT, ticket);
    msg = recvCoapPacket(sock);
    printf("receve packet: %s\n", msg.payload);
  }
  // end listen
  listenCoapPacketEnd(sock);
  return 0;
}