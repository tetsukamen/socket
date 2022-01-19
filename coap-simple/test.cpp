#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "SHA256.h"

int main() {
  // const char* message = "133.1.1.5aaaaaaaaaaaaaaaaa";

  // //	結果格納配列を作成する
  // unsigned int H[INIT_HASH_LENGTH];

  // //	SHA256インスタンスを作成
  // SHA256 sha256;

  // //	パディング処理を実行
  // unsigned char** result = sha256.padding((char*)message);

  // //	ブロックを表示
  // // sha256.print_block(result);

  // //	ハッシュ化を行う
  // sha256.compute(result, H);

  // //	結果を出力する
  // sha256.print_hash(H);

  // //	メモリ開放
  // sha256.free_block(result);

  uint8_t a[10] = "aaaa";
  a[1] = 0x98;
  a[2] = 0x11;
  char b[10];
  for (int i = 0; i < 10; i++) {
    b[i] = (char)(a[i] - 128);
  }

  printf("a:");
  for (int i = 0; i < 10; i++) {
    printf("%#x ", a[i]);
  }
  printf("\n");

  printf("b:");
  for (int i = 0; i < 10; i++) {
    printf("%#x ", b[i]);
  }
  printf("\n");

  printf("b+128:");
  for (int i = 0; i < 10; i++) {
    printf("%#x ", b[i] + 128);
  }
  printf("\n");

  return 0;
}