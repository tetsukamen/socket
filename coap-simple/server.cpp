#include "libcoap.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
  // variables declaration
  struct Message msg;
  char ticket[BUFF_SIZE];
  char data[BUFF_SIZE];

  // start listen
  int sock = listenCoapPacketStart(SERVER_IP, SERVER_PORT);
  printf("Listenting...\n");

  while (1) {
    // receve packet
    msg = recvCoapPacket(sock);
    printf("receve: %s\n", msg.payload);
    if (strcmp(msg.payload, "ticketRequest") == 0) {
      // send ticket
      strcpy(ticket, generateTicket(msg.ip));
      printf("Ticket has issued\n");
      sendCoapPacket(sock, ticket, msg.ip, msg.port, "");
    } else {
      // send response
      int isValid = validateTicket(msg.payload, msg.ip);
      if (isValid) {
        printf("request is valid\n");
        strcpy(data, "sensor-data-000112345\n");
        sendCoapPacket(sock, data, msg.ip, msg.port, "");
      } else {
        printf("ticket is invalid\n");
      }
    }
  }
  listenCoapPacketEnd(sock);
  return 0;
}