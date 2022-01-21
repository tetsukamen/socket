/**
 * @file      SHA256.cpp
 * @brief     SHA256暗号化クラス
 * @note      SHA256アルゴリズムで暗号化を行います。
 * @author    Yoshiteru Ishida
 * @copyright Copyright 2021 Yoshiteru Ishida
 */

#include "SHA256.h"

const unsigned int K[64] = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL,
    0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL, 0xd807aa98UL, 0x12835b01UL,
    0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL,
    0xc19bf174UL, 0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
    0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL, 0x983e5152UL,
    0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL,
    0x06ca6351UL, 0x14292967UL, 0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL,
    0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
    0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL,
    0xd6990624UL, 0xf40e3585UL, 0x106aa070UL, 0x19a4c116UL, 0x1e376c08UL,
    0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL,
    0x682e6ff3UL, 0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
    0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL};

const unsigned int H0[] = {0x6a09e667UL, 0xbb67ae85UL, 0x3c6ef372UL,
                           0xa54ff53aUL, 0x510e527fUL, 0x9b05688cUL,
                           0x1f83d9abUL, 0x5be0cd19UL};

/**
        パディング処理

        処理内容：入力データを64バイトごとに分割し、最後のブロックにビット数を追加します。
        ブロックは動的にメモリを確保します。ブロックを使用し終わったらメモリを開放する必要があります。

        引数：入力データ
        戻り値：ブロック配列
*/
unsigned char** padding(char* input) {
  //	入力データの長さを取得する
  int intLength = strlen(input);

  //	振り分けるブロックの個数を計算する
  //	(MESSAGE_BLOCK_SIZE-1)は切り上げのために必要
  int intBlock =
      (intLength + 9 + (MESSAGE_BLOCK_SIZE - 1)) / MESSAGE_BLOCK_SIZE;
  //	std::cout << "block:" << intBlock << std::endl;

  //	ブロック個数分のポインタを確保する
  unsigned char** output =
      (unsigned char**)malloc(sizeof(char*) * (intBlock + 1));

  int intP = 0;
  for (int intI = 0; intI < intBlock; intI++) {
    //	ブロック個数分のメモリを確保する
    output[intI] = (unsigned char*)malloc(sizeof(char) * MESSAGE_BLOCK_SIZE);

    //	コピーする長さを計算する
    int intCopyLength = intLength - intP;

    //	コピーする長さがブロック長を超える場合はブロック長を設定する
    if (intCopyLength > MESSAGE_BLOCK_SIZE) {
      intCopyLength = MESSAGE_BLOCK_SIZE;
    } else {
      //	コピーする長さがマイナスの場合は0とする
      if (intCopyLength < 0) {
        intCopyLength = 0;
      }
    }

    //	コピーする長さがブロックより短い場合
    if (intCopyLength < MESSAGE_BLOCK_SIZE) {
      //	ブロックをクリアする
      memset(output[intI], 0, sizeof(char) * MESSAGE_BLOCK_SIZE);
    }

    //	入力データをコピーする場合
    if (intCopyLength > 0) {
      //	実際にデータをコピーする
      memcpy(output[intI], &input[intP], sizeof(char) * intCopyLength);

      //	コピーした長さがメッセージブロックより小さい場合はコピーした文字列の終端に0x80を入れる
      if (intCopyLength < MESSAGE_BLOCK_SIZE) {
        output[intI][intCopyLength] = 0x80;
      }
    }

    //	入力データをコピーしない場合
    else {
      //	入力データの長さがMESSAGE_BLOCK_SIZEで割り切れる場合は0x80を追加できていないため最後のブロックの先頭に追加する
      if (intLength % MESSAGE_BLOCK_SIZE == 0) {
        output[intI][0] = 0x80;
      }
    }

    //	最後のブロックの場合
    if (intI == intBlock - 1) {
      //	最後の４バイトに文字列長（ビット）を入れる
      int intBitLength = intLength * 8;

      //	std::cout << "bit:" << intBitLength << std::endl;
      output[intI][MESSAGE_BLOCK_SIZE - 4] =
          (unsigned char)(intBitLength >> 24 & (unsigned char)0xff);
      output[intI][MESSAGE_BLOCK_SIZE - 3] =
          (unsigned char)(intBitLength >> 16 & (unsigned char)0xff);
      output[intI][MESSAGE_BLOCK_SIZE - 2] =
          (unsigned char)(intBitLength >> 8 & (unsigned char)0xff);
      output[intI][MESSAGE_BLOCK_SIZE - 1] =
          (unsigned char)(intBitLength & (unsigned char)0xff);
    }

    intP = intP + MESSAGE_BLOCK_SIZE;
  }

  //	ブロック配列の最後にNULLを入れる
  output[intBlock] = NULL;

  //	printf("\n");

  return output;
}

void print_block_one(unsigned char* block) {
  for (int intJ = 0; intJ < MESSAGE_BLOCK_SIZE; intJ++) {
    printf("%02x", block[intJ]);

    if (intJ % 4 == 3) {
      printf(" ");
    }
    if (intJ % 32 == 31) {
      printf("\n");
    }
  }
}

void print_block(unsigned char** block) {
  int intI = 0;
  while (block[intI] != NULL) {
    printf("%d:\n", intI);

    print_block_one(block[intI]);

    for (int intJ = 0; intJ < MESSAGE_BLOCK_SIZE; intJ++) {
      if (block[intI][intJ] >= 0x20 && block[intI][intJ] < 0x80) {
        printf("%c ", block[intI][intJ]);
      } else {
        printf(". ");
      }

      if (intJ % 4 == 3) {
        printf(" ");
      }
      if (intJ % 32 == 31) {
        printf("\n");
      }
    }

    printf("\n");
    printf("\n");

    intI++;
  }
}

void free_block(unsigned char** block) {
  int intI = 0;
  while (block[intI] != NULL) {
    free(block[intI]);
    intI++;
  }
  free(block);
}

void print_hash(unsigned int* H) {
  for (int intI = 0; intI < INIT_HASH_LENGTH; intI++) {
    print_hex(H[intI]);
    printf(" ");
  }

  printf("\n");
}

void print_hex(unsigned int i) {
  unsigned int h;

  h = (i & 0xff000000) >> 24;
  printf("%02x", h);

  h = (i & 0x00ff0000) >> 16;
  printf("%02x", h);

  h = (i & 0x0000ff00) >> 8;
  printf("%02x", h);

  h = (i & 0x000000ff);
  printf("%02x", h);
}

void print_bin(unsigned int i) {
  unsigned int h;

  h = i;
  for (int intI = 0; intI < 32; intI++) {
    if ((h & 0x80000000) == 0x00000000UL) {
      printf("0");
    } else {
      printf("1");
    }
    h = h << 1;
  }
}

void compute(unsigned char** block, unsigned int* H) {
  //	メッセージの個数をカウントする
  int N = 0;
  while (block[N] != NULL) {
    N++;
  }

  unsigned int W[MESSAGE_BLOCK_SIZE];

  //	Hを初期化する
  memcpy(H, H0, sizeof(int) * INIT_HASH_LENGTH);

  //	メッセージ数分ループする
  for (int i = 0; i < N; i++) {
    //	変数定義
    unsigned int a, b, c, d, e, f, g, h, s0, s1, T1, T2;

    //	1. Prepare the message schedule, { Wt }:
    char* msg = (char*)block[i];

    for (int t = 0; t < MESSAGE_BLOCK_SIZE; t++) {
      if (t < 16) {
        int p = t * 4;
        W[t] = (unsigned int)((msg[p] & 0xff) << 24) |
               (unsigned int)((msg[p + 1] & 0xff) << 16) |
               (unsigned int)((msg[p + 2] & 0xff) << 8) |
               (unsigned int)(msg[p + 3] & 0xff);
      } else {
        W[t] =
            sigma1(W[(t - 2)]) + W[(t - 7)] + sigma0(W[(t - 15)]) + W[(t - 16)];
      }
    }

    //	2. Initialize the eight working variables, a, b, c, d, e, f, g, and h,
    // with the (i-1)st hash value:
    a = H[0];
    b = H[1];
    c = H[2];
    d = H[3];
    e = H[4];
    f = H[5];
    g = H[6];
    h = H[7];

    //	3. For t=0 to 63:
    for (int t = 0; t < MESSAGE_BLOCK_SIZE; t++) {
      T1 = h + SIGMA1(e) + Ch(e, f, g) + K[t] + W[t];
      T2 = SIGMA0(a) + Maj(a, b, c);

      h = g;
      g = f;
      f = e;
      e = d + T1;
      d = c;
      c = b;
      b = a;
      a = T1 + T2;
    }

    //	4. Compute the ith intermediate hash value H(i):
    H[0] = (a + H[0]) & 0xffffffff;
    H[1] = (b + H[1]) & 0xffffffff;
    H[2] = (c + H[2]) & 0xffffffff;
    H[3] = (d + H[3]) & 0xffffffff;
    H[4] = (e + H[4]) & 0xffffffff;
    H[5] = (f + H[5]) & 0xffffffff;
    H[6] = (g + H[6]) & 0xffffffff;
    H[7] = (h + H[7]) & 0xffffffff;
  }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add
//   Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project
//   and select the .sln file
