#include <stdio.h>
#include <sys/resource.h>
#include <time.h>

#include "libcoap.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

#define PROPOSED

int main() {
  // printf("PID: %d\n", getpid());

  // variables declaration
  int N = 25;  // 繰り返し回数
  Message msg;
  uint64_t ticket;
  char data[BUFF_SIZE] = {0};
  char payload[BUFF_SIZE] = {0};
  size_t data_size;
  uint8_t packet[BUFF_SIZE];
  int packetSize;
  struct rusage usage;
  char sendData[BUFF_SIZE] =
      "sensordata00000000001111111111222222222233333333334444444444555555555566"
      "666666667777777777888888888899999999990000000000111111111122222222223333"
      "333333444444444455555555556666666666777777777788888888889999999999000000"
      "000011111111112222222222333333333344444444445555555555666666666677777777"
      "778888888888999999999900000000001111111111222222222233333333334444444444"
      "555555555566666666667777777777888888888899999999990000000000111111111122"
      "222222223333333333444444444455555555556666666666777777777788888888889999"
      "\n";
  FILE *fp;
  long cpu_time;

  // start listen
  int sock = listenCoapPacketStart(SERVER_IP, SERVER_PORT);
  printf("Listenting...\n");

#ifdef PROPOSED
  N = N + 1;
#endif

  for (int i = 0; i < N; i++) {
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

#ifdef PROPOSED
    if (ticket == 0x1) {
      // send ticket
      ticket = generateTicket(msg.ip);
      // printf("Ticket has issued\n");
      createCoapPacket("Issued\n", sizeof("Issued\n"), packet, &packetSize,
                       ticket);
      sendCoapPacket(sock, packet, packetSize, msg.ip, msg.port);
    } else {
      // printf("Got ticket\n");
      // send response
      int isValid = validateTicket(ticket, msg.ip);
      if (isValid == 0) {
#endif
        // printf("request is valid\n");
        strcpy(data, sendData);
        data_size = sizeof(sendData);
        createCoapPacket(data, data_size, packet, &packetSize, ticket);
        sendCoapPacket(sock, packet, packetSize, msg.ip, msg.port);
#ifdef PROPOSED
      } else {
        printf("ticket is invalid\n");
      }
    }
#endif
    // printf("\n");
  }  // end for loop

  // 計測
  printf("繰り返し回数:%d\n", N);
  // cpu時間
  cpu_time = clock();
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
    sscanf(output, "%s %d", str, &vmhwm);
  }
  // printf("vmhwm: %d\n", vmhwm);
  fp = fopen("eval_vmhwm.txt", "a");
  fprintf(fp, "%d\n", vmhwm);
  fclose(fp);

  listenCoapPacketEnd(sock);
  return 0;
}