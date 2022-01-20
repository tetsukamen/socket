#include "libcoap.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
  // variables declaration
  struct Message msg;
  uint64_t ticket;
  char data[BUFF_SIZE] = {0};
  char payload[BUFF_SIZE] = {0};
  size_t data_size;
  uint8_t packet[BUFF_SIZE];
  int packetSize;

  // start listen
  int sock = listenCoapPacketStart(SERVER_IP, SERVER_PORT);
  printf("Listenting...\n");

  while (1) {
    // receve packet
    msg = recvCoapPacket(sock);
    ticket = msg.options[0].value;
    printf("---------------------------------------------\n");
    printf("receive: '%s' from %s %d\n", msg.payload, msg.ip, msg.port);
    printf("ver:%d type:%d TKL:%d code:%d token:%#x\n", msg.version, msg.type,
           msg.tokenLength, msg.code, msg.token);
    for (int i = 0; i < OPTION_LENGTH; i++) {
      printf("option No.%d: delta:%d length:%d value:%#llx\n", i,
             msg.options[i].delta, msg.options[i].length,
             (unsigned long long)msg.options[i].value);
    }
    printf("\n");

// payloadを16進数で表示
#if 0
    for (int i = 0; i < sizeof(payload); i++) {
      printf("%#x ", payload[i]);
    }
    printf("\n");
#endif

    if (ticket == 0x1) {
      // send ticket
      ticket = generateTicket(msg.ip);
      printf("Ticket has issued\n");
      createCoapPacket("Ticket has issued\n", sizeof("Ticket has issued\n"),
                       packet, &packetSize, ticket);
      sendCoapPacket(sock, packet, packetSize, msg.ip, msg.port);
    } else {
      printf("Got ticket\n");
      // send response
      int isValid = validateTicket(ticket, msg.ip);
      if (isValid == 0) {
        printf("request is valid\n");
        strcpy(data, "sensor-data-000112345\n");
        data_size = sizeof("sensor-data-000112345\n");
        createCoapPacket(data, data_size, packet, &packetSize, ticket);
        sendCoapPacket(sock, packet, packetSize, msg.ip, msg.port);
      } else {
        printf("ticket is invalid\n");
      }
    }
    printf("\n");
  }
  listenCoapPacketEnd(sock);
  return 0;
}