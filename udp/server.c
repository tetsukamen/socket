#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define BUF_SIZE 1024

int main()
{
    int sock;
    struct sockaddr_in addr;

    char buf[BUF_SIZE];

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr *)&addr, sizeof(addr));

    memset(buf, 0, sizeof(buf));

    printf("Listening...\n");
    while (1)
    {
        recv(sock, buf, sizeof(buf), 0);
        printf("%s\n", buf);
    }

    close(sock);

    return 0;
}
