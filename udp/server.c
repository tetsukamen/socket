#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SEND_SERVER_ADDR "127.0.0.1"
#define SEND_SERVER_PORT 8081
#define RECV_SERVER_ADDR "127.0.0.1"
#define RECV_SERVER_PORT 8080
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

    printf("Listening...\n");
    while (1)
    {
        recv(recv_sock, recv_buf, sizeof(recv_buf), 0);
        if (strcmp(recv_buf, "end") != 0)
        {
            printf("%s\n", recv_buf);
            sendto(send_sock, "OK", 5, 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
        }
        else
        {
            printf("end communication\n");
            break;
        }
    }

    close(send_sock);
    close(recv_sock);

    return 0;
}
