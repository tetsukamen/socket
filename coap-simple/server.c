#include <stdio.h>
#include <sys/resource.h>
#include <time.h>

#include "libcoap.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
  printf("PID: %d\n", getpid());

  // variables declaration
  Message msg;
  uint64_t ticket;
  char data[BUFF_SIZE] = {0};
  char payload[BUFF_SIZE] = {0};
  size_t data_size;
  uint8_t packet[BUFF_SIZE];
  int packetSize;
  struct rusage usage;

  // start listen
  int sock = listenCoapPacketStart(SERVER_IP, SERVER_PORT);
  printf("Listenting...\n");

  // for (int i = 0; i < 101; i++) {
  // receve packet
  msg = recvCoapPacket(sock);
  ticket = msg.options[0].value;
  // printf("---------------------------------------------\n");
  // printf("receive: '%s' from %s %d\n", msg.payload, msg.ip, msg.port);
  // printf("ver:%d type:%d TKL:%d code:%d token:%#x\n", msg.version,
  // msg.type,
  //        msg.tokenLength, msg.code, msg.token);
  // for (int i = 0; i < OPTION_LENGTH; i++) {
  //   printf("option No.%d: delta:%d length:%d value:%#llx\n", i,
  //          msg.options[i].delta, msg.options[i].length,
  //          (unsigned long long)msg.options[i].value);
  // }
  // printf("\n");

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
    createCoapPacket("Issued\n", sizeof("Issued\n"), packet, &packetSize,
                     ticket);
    sendCoapPacket(sock, packet, packetSize, msg.ip, msg.port);
  } else {
    // printf("Got ticket\n");
    // send response
    int isValid = validateTicket(ticket, msg.ip);
    if (isValid == 0) {
      // printf("request is valid\n");
      strcpy(data, "sensor-data-000112345\n");
      data_size = sizeof("sensor-data-000112345\n");
      createCoapPacket(data, data_size, packet, &packetSize, ticket);
      sendCoapPacket(sock, packet, packetSize, msg.ip, msg.port);
    } else {
      printf("ticket is invalid\n");
    }
  }
  // printf("\n");
  // }

  // 計測
  FILE *fp;

  // cpu時間
  long cpu_time = clock();
  printf("cpu time: %ld\n", cpu_time);
  fp = fopen("eval_cpu.txt", "a");
  fprintf(fp, "%ld\n", cpu_time);
  fclose(fp);

  // 最大メモリ使用量
  char command[128];
  char output[128];
  int vmhwm;
  char str[128];
  sprintf(command, "grep VmHWM /proc/%d/status", getpid());
  if ((fp = popen(command, "r")) == NULL) {
    /*Failure*/
    return 0;
  }
  while (fgets(output, 128, fp) != NULL) {
    //具体的な数値を取得する場合は、sscanf等で読み出し
    sscanf(output,"%s %d",str,&vmhwm);
  }
  printf("vmhwm: %d\n",vmhwm);
  fp = fopen("eval_vmhwm.txt", "a");
  fprintf(fp, "%d\n", vmhwm);
  fclose(fp);

  listenCoapPacketEnd(sock);
  return 0;
}