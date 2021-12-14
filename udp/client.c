#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SEND_SERVER_ADDR "127.0.0.1"
#define SEND_SERVER_PORT 8080
#define RECV_SERVER_ADDR "127.0.0.1"
#define RECV_SERVER_PORT 8081
#define BUF_SIZE 1024

int main()
{
   // declaration
   int send_sock, recv_sock;
   struct sockaddr_in send_addr, recv_addr;
   char send_buf[BUF_SIZE], recv_buf[BUF_SIZE];

   // create socket
   send_sock = socket(AF_INET, SOCK_DGRAM, 0);
   recv_sock = socket(AF_INET, SOCK_DGRAM, 0);

   // set address
   send_addr.sin_family = AF_INET;
   send_addr.sin_port = htons(SEND_SERVER_PORT);
   send_addr.sin_addr.s_addr = inet_addr(SEND_SERVER_ADDR);
   recv_addr.sin_family = AF_INET;
   recv_addr.sin_port = htons(RECV_SERVER_PORT);
   recv_addr.sin_addr.s_addr = inet_addr(RECV_SERVER_ADDR);

   // bind socket to listen
   bind(recv_sock, (struct sockaddr *)&recv_addr, sizeof(recv_addr));

   memset(recv_buf, 0, sizeof(recv_buf));

   // start communication
   printf("Input Message...\n");
   while (1)
   {
      /* サーバーに送る文字列を取得 */
      scanf("%s", send_buf);
      if (strcmp(send_buf, "finish") != 0)
      {
         sendto(send_sock, send_buf, 5, 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
         recv(recv_sock, recv_buf, sizeof(recv_buf), 0);
         printf("response: %s\n", recv_buf);
      }
      else
      {
         break;
      }
   }

   close(send_sock);
   close(recv_sock);

   return 0;
}