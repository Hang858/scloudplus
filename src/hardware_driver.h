#ifndef HARDWARE_DRIVER_H
#define HARDWARE_DRIVER_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief 算子3. 哈希
 * 
 * @param alg        输入：决定使用的哈希算法类型。（1:SHA3256; 2:SM3  3:SHAKE256）
 * @param mode       输入：决定使用的哈希算法模式。(0:普通； 1:哈希链(LMS)；2:哈希链(SPHINCS))
 * @param n          输入：输出哈希值的长度 (字节)。
 * @param input      输入：待哈希的数据缓冲区指针。
 * @param input_len  输入：待哈希数据的长度 (字节)。
 * @param output     输出：存储哈希结果的缓冲区指针。
 * @return           0: 成功; -1: 失败。
 */

#define OP_ALG_SHA3_256   0
#define OP_ALG_SHA3_512   1
#define OP_ALG_SM3      2
#define OP_ALG_SHAKE128 3    
#define OP_ALG_SHAKE256 4
#define OP_MODE_NORMAL 0
#define OP_MODE_LMS 1
#define OP_MODE_SPHINCS 2
#define INITIAL_HW_BUFFER_SIZE 4096
#define INITIAL_HW_OUTPUT_SIZE 1024

int OP_hash(uint8_t alg, uint8_t mode, int n, void *input, int input_len, void *output);
#endif