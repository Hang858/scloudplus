#include "matrix.h"
#include "param.h"
#include <stdint.h>
#include <string.h>
#include "vector_op.h"
void scloudplus_add(uint16_t *in0, uint16_t *in1, int len, uint16_t *out)
{
	for (int i = 0; i < len; i++)
	{
		out[i] = (in0[i] + in1[i]) & 0xFFF;
	}
}
void scloudplus_sub(uint16_t *in0, uint16_t *in1, int len, uint16_t *out)
{
	for (int i = 0; i < len; i++)
	{
		out[i] = (in0[i] - in1[i]) & 0xFFF;
	}
}

// 计算 C * S^T
// C: mbar x n
// S: nbar x n 
// Out: mbar x nbar
void scloudplus_mul_cs(uint16_t *C, uint16_t *S, uint16_t *out)
{
	memset(out, 0, scloudplus_mbar * scloudplus_nbar * sizeof(uint16_t));
    uint16_t q = (1 << scloudplus_logq);
    uint16_t res;
    for (int i = 0; i < scloudplus_mbar; ++i) 
    {
        for (int j = 0; j < scloudplus_nbar; ++j) 
        {
            OP_vector_mul(&res, 
                          &C[i * scloudplus_n], 
                          &S[j * scloudplus_n], 
                          scloudplus_n, 
                          q);
            
            out[i * scloudplus_nbar + j] = res;
        }
    }
}

// 计算 S * B + E
// S: mbar x m
// B: m x nbar
// E: mbar x nbar
void scloudplus_mul_add_sb_e(const uint16_t *S, const uint16_t *B,
							 const uint16_t *E, uint16_t *out)
{
	memcpy(out, E, scloudplus_mbar * scloudplus_nbar * sizeof(uint16_t));
    uint16_t q = (1 << scloudplus_logq);
    uint16_t res;
    uint16_t B_Trans[scloudplus_nbar * scloudplus_m];
    matrix_transpose(B, B_Trans, scloudplus_m, scloudplus_nbar);
	for (int i = 0; i < scloudplus_mbar; ++i) 
    {
        for (int j = 0; j < scloudplus_nbar; ++j) 
        {
            OP_vector_mul(&res, &S[i * scloudplus_m], &B_Trans[j * scloudplus_m], scloudplus_m, q);
            out[i * scloudplus_nbar + j] += res;
        }
    }
}
