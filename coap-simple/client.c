#include <sys/resource.h>
#include <time.h>

#include "libcoap.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define CLIENT_IP "127.0.0.1"
#define CLIENT_PORT 8081

int main() {
  // variables declaration
  Message msg;
  uint64_t ticket;
  uint8_t packet[BUFF_SIZE];
  int packetSize;
  struct rusage usage;

  // start listen
  int sock = listenCoapPacketStart(CLIENT_IP, CLIENT_PORT);

  // get ticket
  createCoapPacket("ticketRequest pp\n", sizeof("ticketRequest\n"), packet,
                   &packetSize, 0x1);
  sendCoapPacket(sock, packet, packetSize, SERVER_IP, SERVER_PORT);
  printf("send ticket request\n\n");
  msg = recvCoapPacket(sock);
  ticket = msg.options[0].value;
  printf("---------------------------------------------\n");
  printf("receve ticket: %#llx\n\n", ticket);

  // get data request 3 times
  for (int i = 0; i < 100; i++) {
    // printf("send request\n\n");
    createCoapPacket("GET /data\n", sizeof("GET /data\n"), packet, &packetSize,
                     ticket);
    sendCoapPacket(sock, packet, packetSize, SERVER_IP, SERVER_PORT);
    msg = recvCoapPacket(sock);
    // printf("---------------------------------------------\n");
    // printf("receve packet: %s\n\n", msg.payload);
  }
  // end listen
  listenCoapPacketEnd(sock);

  long cpu_time = clock();
  printf("cpu time: %ld\n", cpu_time);

  getrusage(RUSAGE_SELF, &usage);
  printf("maxrss: %ld\n", usage.ru_maxrss);

  return 0;
}