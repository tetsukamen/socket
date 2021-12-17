#include "libcoap.h"

int listenCoapPacketStart(char *ip, int port) {
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(ip);
  addr.sin_port = htons(port);
  bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  return sock;
};

void listenCoapPacketEnd(int sock) { close(sock); };

int sendCoapPacket(char *payload, char *ip, int port, char *ticket) {
  printf("send packet");
  // create socket
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  // sed addr
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = addr.sin_addr.s_addr = inet_addr(ip);
  addr.sin_port = htons(port);
  // create coap packet
  char packet[DATA_SIZE] = "00000000";
  // TODO: packet作成コードの実装
  // send
  int ret = sendto(sock, packet, sizeof(packet), 0, (struct sockaddr *)&addr,
                   sizeof(addr));
  return ret;
};

Message recvCoapPacket(int sock) {
  // 変数宣言
  char buf[BUFF_SIZE];
  struct sockaddr_in from;
  socklen_t addrlen;
  addrlen = sizeof(from);
  memset(buf, 0, sizeof(buf));

  // パケット受け取り
  recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&from, &addrlen);

  // ペイロードの取り出し
  char payload[BUFF_SIZE];
  strcpy(payload, buf);  // TODO: ペイロードを取り出す処理を書く

  // 送信元の情報を取り出す
  char ip[15];
  inet_ntop(AF_INET, &from.sin_addr, ip, sizeof(ip));

  // Messageオブジェクトを作成
  struct Message msg = {"", *ip, ntohs(from.sin_port)};
  strcpy(msg.payload, payload);

  return msg;
};

char *SHA(char *ip, char *secret) {
  char *hash = "bbbbbbbbb";
  return hash;
}

char *generateTicket(char *ip) {
  char *ticket;
  ticket = SHA(ip, SECRET);
  return ticket;
};

int validateTicket(char *ticket, char *ip) {
  char *valid = SHA(ip, SECRET);
  if (valid == ticket) {
    return 1;
  } else {
    return 0;
  }
};