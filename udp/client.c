#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080
#define BUF_SIZE 1024

int main()
{
   int sock;
   struct sockaddr_in addr;
   char send_buf[BUF_SIZE], recv_buf;

   sock = socket(AF_INET, SOCK_DGRAM, 0);

   addr.sin_family = AF_INET;
   addr.sin_port = htons(SERVER_PORT);
   addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

   printf("Input Message...\n");
   while (1)
   {
      /* サーバーに送る文字列を取得 */
      scanf("%s", send_buf);
      if (strcmp(send_buf, "finish") != 0)
      {
         sendto(sock, send_buf, 5, 0, (struct sockaddr *)&addr, sizeof(addr));
      }
      else
      {
         break;
      }
   }

   close(sock);

   return 0;
}