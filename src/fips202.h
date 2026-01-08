#ifndef FIPS202_H
#define FIPS202_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "hardware_driver.h"
#define SHAKE128_RATE 168
#define SHAKE256_RATE 136
#define SHA3_256_RATE 136
#define SHA3_512_RATE 72

typedef struct
{
	uint64_t s[25];
	unsigned int pos;

	// 新增：用于硬件算子替换
#ifdef USE_HARDWARE_HASH
	uint8_t *buffer;         // 分配的缓冲区
	size_t buf_len;    		 // 当前已存入的数据长度	
	size_t buf_cap;   	     // 缓冲区容量
	// 输出缓冲区，用于 squeeze
	uint8_t *out_buffer;     
    size_t out_buf_len;      // 已经缓存了多少输出数据
    size_t out_buf_pos;      // 已经读走了多少
    size_t out_buf_cap;      // 输出缓存总容量
#endif

} keccak_state;

void shake128_init(keccak_state *state);
void shake128_absorb(keccak_state *state, const uint8_t *in, size_t inlen);
void shake128_finalize(keccak_state *state);
void shake128_squeeze(uint8_t *out, size_t outlen, keccak_state *state);
void shake128_absorb_once(keccak_state *state, const uint8_t *in, size_t inlen);
void shake128_squeezeblocks(uint8_t *out, size_t nblocks, keccak_state *state);
void shake256_init(keccak_state *state);
void shake256_absorb(keccak_state *state, const uint8_t *in, size_t inlen);
void shake256_finalize(keccak_state *state);
void shake256_squeeze(uint8_t *out, size_t outlen, keccak_state *state);
void shake256_absorb_once(keccak_state *state, const uint8_t *in, size_t inlen);
void shake256_squeezeblocks(uint8_t *out, size_t nblocks, keccak_state *state);
void shake128(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen);
void shake256(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen);

void scloudplus_F(unsigned char *output, unsigned long long outlen,
				  const unsigned char *input, unsigned long long inlen);
void scloudplus_K(unsigned char *output, unsigned long long outlen,
				  const unsigned char *input, unsigned long long inlen);
void scloudplus_H(unsigned char *output, const unsigned char *input,
				  unsigned long long inlen);
void scloudplus_G(unsigned char *output, const unsigned char *input,
				  unsigned long long inlen);

void sha3_512(uint8_t h[64], const uint8_t *in, size_t inlen);
void sha3_256(uint8_t h[32], const uint8_t *in, size_t inlen);

// 释放内存
void keccak_state_free(keccak_state *state);

#endif
