#include "libcoap.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main()
{
  // variables declaration
  struct Message msg;
  uint8_t ticket;
  char data[BUFF_SIZE];
  char payload[BUFF_SIZE] = {0};

  // start listen
  int sock = listenCoapPacketStart(SERVER_IP, SERVER_PORT);
  printf("Listenting...\n");

  while (1)
  {
    // receve packet
    msg = recvCoapPacket(sock);
    ticket = msg.options[0].value;
    printf("---------------------------------------------\n");
    printf("receve: %s\n", msg.payload);
    printf("ver:%d type:%d TKL:%d code:%d token:%#x\n", msg.version, msg.type, msg.tokenLength, msg.code, msg.token);
    for (int i = 0; i < OPTION_LENGTH; i++)
    {
      printf("option No.%d: delta:%d length:%d value:%#x\n", i, msg.options[i].delta, msg.options[i].length, msg.options[i].value);
    }

// payloadを16進数で表示
#if 0
    for (int i = 0; i < sizeof(payload); i++) {
      printf("%#x ", payload[i]);
    }
    printf("\n");
#endif

    if (ticket == 0x1)
    {
      // send ticket
      ticket = generateTicket(msg.ip);
      printf("Ticket has issued\n");
      sendCoapPacket(sock, "Ticket has issued", sizeof("Ticket has issued"), msg.ip, msg.port, ticket);
    }
    else
    {
      printf("Got ticket\n");
      // send response
      int isValid = validateTicket(ticket, msg.ip);
      if (isValid == 0)
      {
        printf("request is valid\n");
        strcpy(data, "sensor-data-000112345\n");

        int ret = sendCoapPacket(sock, data, sizeof(data), msg.ip, msg.port);
        printf("%d\n", ret);
      }
      else
      {
        printf("ticket is invalid\n");
      }
    }
  }
  listenCoapPacketEnd(sock);
  return 0;
}