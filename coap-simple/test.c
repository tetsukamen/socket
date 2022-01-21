#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "SHA256.h"

int main() {
  const char* message = "133.1.1.5aaaaaaaaaaaaaaaaa";

  //	結果格納配列を作成する
  unsigned int H[INIT_HASH_LENGTH];

  // //	SHA256インスタンスを作成
  SHA256 sha256 = {
      print_hex,  print_bin,  print_block_one, print_block,
      print_hash, free_block, padding,         compute,
  };

  //	パディング処理を実行
  unsigned char** result = sha256.padding((char*)message);

  //	ブロックを表示
  sha256.print_block(result);

  //	ハッシュ化を行う
  sha256.compute(result, H);

  //	結果を出力する
  sha256.print_hash(H);

  //	メモリ開放
  sha256.free_block(result);

  return 0;
}