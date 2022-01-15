/**
 * @file      SHA256.cpp
 * @brief     SHA256暗号化クラス
 * @note      SHA256アルゴリズムで暗号化を行います。
 * @author    Yoshiteru Ishida
 * @copyright Copyright 2021 Yoshiteru Ishida
 */

#include "SHA256.h"

#include <iomanip>
#include <iostream>

#define MESSAGE_BLOCK_SIZE 64

/**
        パディング処理

        処理内容：入力データを64バイトごとに分割し、最後のブロックにビット数を追加します。
        ブロックは動的にメモリを確保します。ブロックを使用し終わったらメモリを開放する必要があります。

        引数：入力データ
        戻り値：ブロック配列
*/
unsigned char** SHA256::padding(char* input) {
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
      memset(output[intI], NULL, sizeof(char) * MESSAGE_BLOCK_SIZE);
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

  //	std::cout << std::endl;

  return output;
}

void SHA256::print_block_one(unsigned char* block) {
  for (int intJ = 0; intJ < MESSAGE_BLOCK_SIZE; intJ++) {
    std::cout << std::hex << std::setw(2) << std::setfill('0')
              << (unsigned short int)(block[intJ]);

    if (intJ % 4 == 3) {
      std::cout << " ";
    }
    if (intJ % 32 == 31) {
      std::cout << std::endl;
    }
  }
  std::cout << std::dec;
}

void SHA256::print_block(unsigned char** block) {
  int intI = 0;
  while (block[intI] != NULL) {
    std::cout << intI << ":" << std::endl;

    print_block_one(block[intI]);

    for (int intJ = 0; intJ < MESSAGE_BLOCK_SIZE; intJ++) {
      if (block[intI][intJ] >= 0x20 && block[intI][intJ] < 0x80) {
        std::cout << (block[intI][intJ]) << " ";
      } else {
        std::cout << ". ";
      }

      if (intJ % 4 == 3) {
        std::cout << " ";
      }
      if (intJ % 32 == 31) {
        std::cout << std::endl;
      }
    }

    std::cout << std::endl;
    std::cout << std::endl;

    intI++;
  }
}

void SHA256::free_block(unsigned char** block) {
  int intI = 0;
  while (block[intI] != NULL) {
    free(block[intI]);
    intI++;
  }
  free(block);
}

void SHA256::print_hash(unsigned int* H) {
  for (int intI = 0; intI < INIT_HASH_LENGTH; intI++) {
    print_hex(H[intI]);
    std::cout << " ";
  }

  std::cout << std::endl;
}

void SHA256::print_hex(unsigned int i) {
  unsigned int h;

  h = (i & 0xff000000) >> 24;
  std::cout << std::hex << std::setw(2) << std::setfill('0')
            << (unsigned short int)(h);

  h = (i & 0x00ff0000) >> 16;
  std::cout << std::hex << std::setw(2) << std::setfill('0')
            << (unsigned short int)(h);

  h = (i & 0x0000ff00) >> 8;
  std::cout << std::hex << std::setw(2) << std::setfill('0')
            << (unsigned short int)(h);

  h = (i & 0x000000ff);
  std::cout << std::hex << std::setw(2) << std::setfill('0')
            << (unsigned short int)(h);
}

void SHA256::print_bin(unsigned int i) {
  unsigned int h;

  h = i;
  for (int intI = 0; intI < 32; intI++) {
    if ((h & 0x80000000) == 0x00000000UL) {
      std::cout << "0";
    } else {
      std::cout << "1";
    }
    h = h << 1;
  }
}

void SHA256::compute(unsigned char** block, unsigned int* H) {
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
