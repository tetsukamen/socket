#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
  char data[128] = {0};
  strcpy(data, "sensor-data-000112345\n");
  for (int i = 0; i < sizeof("sensor-data-000112345\n"); i++)
  {
    printf("%#x %c\n", data[i], data[i]);
  }
}