#include "libcoap.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define CLIENT_IP "127.0.0.1"
#define CLIENT_PORT 8081

int main()
{
  // variables declaration
  struct Message msg;
  uint8_t ticket;

  // start listen
  int sock = listenCoapPacketStart(CLIENT_IP, CLIENT_PORT);

  printf("---------------------------------------------\n");
  // get ticket
  sendCoapPacket(sock, "ticketRequest\n", sizeof("ticketRequest\n"), SERVER_IP,
                 SERVER_PORT, 0x1);
  printf("send ticket request\n");
  msg = recvCoapPacket(sock);
  ticket = msg.options[0].value;
  printf("receve ticket: %#x\n", ticket);

  // get data request 3 times
  // for (int i = 0; i < 3; i++) {
  printf("---------------------------------------------\n");
  printf("send request\n");
  sendCoapPacket(sock, "GET /data", sizeof("GET /data"), SERVER_IP,
                 SERVER_PORT,
                 ticket);
  msg = recvCoapPacket(sock);
  printf("receve packet: %s\n", msg.payload);
  // }
  // end listen
  listenCoapPacketEnd(sock);
  return 0;
}