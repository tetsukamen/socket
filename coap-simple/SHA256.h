/**
 * @file      SHA256.h
 * @brief     SHA256暗号化クラス
 * @note      SHA256アルゴリズムで暗号化を行います。
 * @author    Yoshiteru Ishida
 * @copyright Copyright 2021 Yoshiteru Ishida
 */

#define MESSAGE_BLOCK_SIZE 64
#define INIT_HASH_LENGTH 8

#define ROTR(x, n) ((x >> n | x << (32 - n)))
#define SHR(x, n) ((x >> n))
#define Ch(x, y, z) ((x & y) ^ (~x & z))
#define Maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SIGMA0(x) ((ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22)))
#define SIGMA1(x) ((ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25)))
#define sigma0(x) ((ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3)))
#define sigma1(x) ((ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10)))

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

class SHA256 {
 public:
  void print_hex(unsigned int i);
  void print_bin(unsigned int i);
  void print_block_one(unsigned char* block);
  void print_block(unsigned char** block);
  void print_hash(unsigned int* H);

  void free_block(unsigned char** block);

  unsigned char** padding(char* input);
  void compute(unsigned char** block, unsigned int* H);

 private:
};
