// #include <arpa/inet.h>
// #include <errno.h>
// #include <netinet/in.h>
// #include <sys/resource.h>
// #include <sys/socket.h>
#include <stdio.h>
// #include <time.h>
#include <unistd.h>

// #include "SHA256.h"

int main() {
#if 0
  // const char* message = "133.1.1.5aaaaaaaaaaaaaaaaa";

  // //	結果格納配列を作成する
  // unsigned int H[INIT_HASH_LENGTH];

  // // //	SHA256インスタンスを作成
  // SHA256 sha256 = {
  //     print_hex,  print_bin,  print_block_one, print_block,
  //     print_hash, free_block, padding,         compute,
  // };

  // //	パディング処理を実行
  // unsigned char** result = sha256.padding((char*)message);

  // //	ブロックを表示
  // sha256.print_block(result);

  // //	ハッシュ化を行う
  // sha256.compute(result, H);

  // //	結果を出力する
  // sha256.print_hash(H);

  // //	メモリ開放
  // sha256.free_block(result);
#endif

  // printf("PID: %d\n", getpid());

  // long cpu_time = clock();
  // printf("cpu time: %ld\n", cpu_time);

  // struct rusage usage;
  // getrusage(RUSAGE_SELF, &usage);
  // printf("maxrss: %ld\n", usage.ru_maxrss);

  char dummy[2];
  fgets(dummy, 2, stdin);

  return 0;
}