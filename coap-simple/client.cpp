#include "libcoap.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define CLIENT_IP "127.0.0.1"
#define CLIENT_PORT 8081

int main() {
  // start listen
  // int recv_sock = listenCoapPacketStart(CLIENT_IP, CLIENT_PORT);
  // get ticket
  printf("send ticket request\n");
  int ret = sendCoapPacket("ticketRequest", SERVER_IP, SERVER_PORT, "");

  // struct Message msg;
  // msg = recvCoapPacket(recv_sock);
  // char *ticket;
  // ticket = msg.payload;
  // // get data request 3 times
  // for (int i = 0; i < 3; i++) {
  //   printf("send request\n");
  //   sendCoapPacket("request", SERVER_IP, SERVER_PORT, ticket);
  //   msg = recvCoapPacket(recv_sock);
  //   printf("%s", msg.payload);
  // }
  // // end listen
  // listenCoapPacketEnd(recv_sock);
  return 0;
}