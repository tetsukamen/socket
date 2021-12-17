#include "libcoap.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
  // start listen
  int recv_sock = listenCoapPacketStart(SERVER_IP, SERVER_PORT);
  struct Message msg;
  char *ticket;
  char data[DATA_SIZE];

  printf("server started\n");

  while (1) {
    // receve packet
    msg = recvCoapPacket(recv_sock);
    printf("%s\n", msg.payload);
    // if (msg.payload == "ticketRequest") {  // send ticket
    //   ticket = generateTicket(msg.ip);
    //   printf("ticket needed\n");
    //   sendCoapPacket(ticket, msg.ip, msg.port, "");
    // } else {  // send response
    //   int isValid = validateTicket(msg.ticket, msg.ip);
    //   if (isValid) {
    //     printf("request is valid");
    //     sendCoapPacket(data, msg.ip, msg.port, "");
    //   }
    // }
  }
  listenCoapPacketEnd(recv_sock);
  return 0;
}