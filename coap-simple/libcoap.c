#include "libcoap.h"

int listenCoapPacketStart(const char *ip, int port) {
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(ip);
  addr.sin_port = htons(port);
  int ret = bind(sock, (const struct sockaddr *)&addr, sizeof(addr));
  if (ret == -1) {
    perror("bind: ");
  }
  return sock;
};

void listenCoapPacketEnd(int sock) { close(sock); };

void createCoapPacket(const char *payload, int payload_size, uint8_t *packet,
                      int *packetSize, uint64_t ticket) {
  // create coap packet
  uint8_t *p = packet;
  uint16_t _packetSize = 0;
  uint8_t token[COAP_TOKEN_SIZE];
  // Coap header
  *p = 0x01 << 6;                  // Coap Version
  *p |= 0x01 << 4;                 // Type
  *p++ |= COAP_TOKEN_SIZE & 0x0F;  // Token Length: 1 byte
  *p++ = 1;  // Code: 00000001(定義されてない適当な値)
  *p++ = 0;  // message id upper
  *p++ = 1;  // message id lower (全体で00000000 00000001)
  _packetSize += COAP_HEADER_SIZE;

  // Coap token
  token[0] = 0x0F;  // Token is 00001111
  *p = token[0];
  p += COAP_TOKEN_SIZE;
  _packetSize += COAP_TOKEN_SIZE;

  // Make ticket option
  *p = 0x01 << 4;  // set option delta 0001
  *p++ |= 0x04;    // set option length 0001
  // set option value
  *p++ = (ticket & 0xff000000) >> 24;
  *p++ = (ticket & 0x00ff0000) >> 16;
  *p++ = (ticket & 0x0000ff00) >> 8;
  *p++ = (ticket & 0x000000ff);
  _packetSize += 5;

  // Make dummy option
  *p = 0x01 << 4;  // set option delta 0001
  *p++ |= 0x01;    // set option length 0001
  *p++ = 0xFC;     // set option value
  _packetSize += 2;

  // Payload marker
  *p++ = 0xFF;
  _packetSize++;

  // Coap Payload
  memcpy(p, payload, payload_size);
  p += payload_size;
  _packetSize += payload_size;

  *packetSize = _packetSize;
}

int sendCoapPacket(int sock, uint8_t *packet, int packetSize,
                   const char *dist_ip, int dist_port) {
  // dist addr
  struct sockaddr_in dist_addr;
  memset(&dist_addr, 0, sizeof(dist_addr));
  dist_addr.sin_family = AF_INET;
  dist_addr.sin_addr.s_addr = inet_addr(dist_ip);
  dist_addr.sin_port = htons(dist_port);

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
  int ret = (int)sendto(sock, packet, packetSize, 0,
                        (struct sockaddr *)&dist_addr, sizeof(dist_addr));

  if (ret == -1) {
    perror("sendto: ");
  }
  return ret;
};

Message recvCoapPacket(int sock) {
  // 変数宣言
  uint8_t buf[BUFF_SIZE];
  memset(buf, 0, BUFF_SIZE);
  struct sockaddr_in from;
  memset(&from, 0, sizeof(from));
  socklen_t addrlen;
  addrlen = sizeof(from);
  size_t bodySize = 0;
  Message msg;

  // パケット受け取り
  recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&from, &addrlen);
  // if (ret != -1) {
  //   perror("recvfrom: ");
  // }

// 16進数で表示
#if 0
  for (int i = 0; i < sizeof(buf); i++) {
    printf("%#x ", buf[i]);
  }
  printf("\n");
#endif

  // ペイロードの取り出し
  uint8_t payload[BUFF_SIZE] = {0};
  uint8_t *p = payload;
  int k = 0;
  for (k = 0; buf[k] != 0xa; k++) {
    payload[k] = buf[k];
  }
  payload[k] = 0xa;

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
  memset(msg.payload, 0, BUFF_SIZE);  // 初期化
  memcpy(msg.payload, p, bodySize);

  return msg;
};

uint64_t SHA(char *ip, const char *secret) {
  // メッセージ生成
  char message[25];
  strcpy(message, ip);
  strcat(message, secret);

  unsigned int H[INIT_HASH_LENGTH];  //	結果格納配列を作成する

  SHA256 sha256 = {
      print_hex,  print_bin,  print_block_one, print_block,
      print_hash, free_block, padding,         compute,
  };  //	SHA256インスタンスを作成

  //	パディング処理を実行
  unsigned char **result = sha256.padding((char *)message);

  // sha256.print_block(result); //	ブロックを表示

  sha256.compute(result, H);  //	ハッシュ化を行う

  uint64_t hash = H[0];
  // printf("%#llx\n", hash);

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
