/**
 * @file      SHA256.h
 * @brief     SHA256暗号化クラス
 * @note      SHA256アルゴリズムで暗号化を行います。
 * @author    Yoshiteru Ishida
 * @copyright Copyright 2021 Yoshiteru Ishida
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct {
  void (*print_hex)(unsigned int i);
  void (*print_bin)(unsigned int i);
  void (*print_block_one)(unsigned char* block);
  void (*print_block)(unsigned char** block);
  void (*print_hash)(unsigned int* H);

  void (*free_block)(unsigned char** block);

  unsigned char** (*padding)(char* input);
  void (*compute)(unsigned char** block, unsigned int* H);
} SHA256;

void print_hex(unsigned int i);
void print_bin(unsigned int i);
void print_block_one(unsigned char* block);
void print_block(unsigned char** block);
void print_hash(unsigned int* H);

void free_block(unsigned char** block);
unsigned char** padding(char* input);
void compute(unsigned char** block, unsigned int* H);