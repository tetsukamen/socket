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

int sendCoapPacket(int sock, char *payload, int payload_size, char *dist_ip,
                   int dist_port, uint64_t ticket) {
  // dist addr
  struct sockaddr_in dist_addr;
  memset(&dist_addr, 0, sizeof(dist_addr));
  dist_addr.sin_family = AF_INET;
  dist_addr.sin_addr.s_addr = inet_addr(dist_ip);
  dist_addr.sin_port = htons(dist_port);
  // create coap packet
  uint8_t packet[BUFF_SIZE];
  uint8_t *p = packet;
  uint16_t packetSize = 0;
  uint8_t token[COAP_TOKEN_SIZE];
  // Coap header
  *p = 0x01 << 6;                  // Coap Version
  *p |= 0x01 << 4;                 // Type
  *p++ |= COAP_TOKEN_SIZE & 0x0F;  // Token Length: 1 byte
  *p++ = 1;  // Code: 00000001(定義されてない適当な値)
  *p++ = 0;  // message id upper
  *p++ = 1;  // message id lower (全体で00000000 00000001)
  packetSize += COAP_HEADER_SIZE;

  // Coap token
  token[0] = 0x0F;  // Token is 00001111
  *p = token[0];
  p += COAP_TOKEN_SIZE;
  packetSize += COAP_TOKEN_SIZE;

  // Make ticket option
  *p = 0x01 << 4;  // set option delta 0001
  *p++ |= 0x02;    // set option length 0001
  // set option value
  *p++ = (ticket & 0x0000ff00) >> 8;
  *p++ = (ticket & 0x000000ff);
  packetSize += 3;

  // Make dummy option
  *p = 0x01 << 4;  // set option delta 0001
  *p++ |= 0x01;    // set option length 0001
  *p++ = 0xFC;     // set option value
  packetSize += 2;

  // Payload marker
  *p++ = 0xFF;
  packetSize++;

  // Coap Payload
  memcpy(p, payload, payload_size);
  p += payload_size;
  packetSize += payload_size;

// 16進数で表示
#if 0
  for (int i = 0; i < packetSize; i++) {
    printf("%#x ", packet[i]);
  }
  printf("\n");
#endif
// 文字で表示
#if 0
  // Print packet
  printf("%d\n", packetSize);
  for (int j = 0; j < packetSize; j++) {
    printf("%c    ", packet[j]);
  }
  printf("\n");
#endif

  // send
  int ret = sendto(sock, packet, sizeof(packet), 0,
                   (struct sockaddr *)&dist_addr, sizeof(dist_addr));
  return ret;
};

Message recvCoapPacket(int sock) {
  // 変数宣言
  uint8_t buf[BUFF_SIZE] = {0};
  struct sockaddr_in from;
  socklen_t addrlen;
  addrlen = sizeof(from);
  size_t bodySize = 0;
  struct Message msg;

  // パケット受け取り
  recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&from, &addrlen);

  // ペイロードの取り出し
  uint8_t payload[BUFF_SIZE] = {0};
  uint8_t *p = payload;
  int k = 0;
  for (k = 0; buf[k] != 0xa; k++) {
    payload[k] = buf[k];
  }
  payload[k] = 0xa;

// 16進数で表示
#if 0
  for (int j = 0; j < sizeof(payload); j++) {
    printf("%#x ", payload[j]);
  }
  printf("\n");
#endif
// 文字で表示
#if 0
  for (int j = 0; j < sizeof(payload); j++) {
    printf("%c    ", payload[j]);
  }
  printf("\n");

#endif

  // CoAPパケットのパース
  // CoAP Version
  msg.version = (int)(*p & 0xC0) >> 6;
  // Type
  msg.type = (int)(*p & 0x30) >> 4;
  // Token
  msg.tokenLength = (int)*p++ & 0x0F;
  // Code
  msg.code = (int)*p++ & 0xFF;
  // message id
  msg.messageIdUpper = *p++;
  msg.messageIdLower = *p++;
  // token
  msg.token = *p++;

  // parse option
  for (int i = 0; i < OPTION_LENGTH; i++) {
    msg.options[i].delta = (int)((0xf0 & *p) >> 4);
    msg.options[i].length = (int)(0x0f & *p++);
    msg.options[i].value = *p++;
    for (int j = 1; j < msg.options[i].length; j++) {
      msg.options[i].value = msg.options[i].value << 8;
      msg.options[i].value |= *p++;
    }
  }

  // ヘッダをスキップ
  while (*p != 0xff) {
    p++;
  }
  p++;

  // bodySizeの計算
  for (uint8_t *i = p; *i != 0xa; i++) {
    bodySize++;
  }

  // 送信元の情報を取り出す
  char ip[15];
  inet_ntop(AF_INET, &from.sin_addr, ip, sizeof(ip));

  // Messageオブジェクトを作成

  msg.port = ntohs(from.sin_port);
  strcpy(msg.ip, ip);
  memcpy(msg.payload, p, bodySize);

  return msg;
};

uint64_t SHA(char *ip, char *secret) {
  uint64_t hash = 0x8966;
  return hash;
}

uint64_t generateTicket(char *ip) {
  uint64_t ticket;
  ticket = SHA(ip, SECRET);
  return ticket;
};

int validateTicket(uint64_t ticket, char *ip) {
  uint64_t valid = SHA(ip, SECRET);
  if (valid == ticket) {
    return 0;
  } else {
    return -1;
  }
};
