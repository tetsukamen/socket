#include "libcoap.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main()
{
  // variables declaration
  struct Message msg;
  char ticket[BUFF_SIZE];
  char data[BUFF_SIZE];
  char payload[BUFF_SIZE] = {0};

  // start listen
  int sock = listenCoapPacketStart(SERVER_IP, SERVER_PORT);
  printf("Listenting...\n");

  while (1)
  {
    // receve packet
    msg = recvCoapPacket(sock);
    printf("receve: %s\n", msg.payload);
    printf("option: delta:%d length:%d value:%#x\n", msg.options[0].delta, msg.options[0].length, msg.options[0].value);

    // for (int i = 0; i < sizeof(payload); i++) {
    //   printf("%#x ", payload[i]);
    // }
    printf("\n");

    // if (strcmp(payload, "ticketRequest") == 0) {
    //   // send ticket
    //   strcpy(ticket, generateTicket(msg.ip));
    //   printf("Ticket has issued\n");
    //   sendCoapPacket(sock, ticket, 9, msg.ip, msg.port, "");
    // } else {
    //   printf("Got ticket\n");
    //   // send response
    //   int isValid = validateTicket(payload, msg.ip);
    //   if (isValid) {
    //     printf("request is valid\n");
    //     strcpy(data, "sensor-data-000112345\n");
    //     sendCoapPacket(sock, data, 21, msg.ip, msg.port, "");
    //   } else {
    //     printf("ticket is invalid\n");
    //   }
    // }
  }
  listenCoapPacketEnd(sock);
  return 0;
}