#include "matrix.h"
#include "param.h"
#include <string.h>
#include "matrix_op.h"
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
	int i, j, k;
    matrix8x8_t R_c, R_s, R_res, R_acc;

    for (i = 0; i < scloudplus_mbar; i += 8) 
    {
        for (j = 0; j < scloudplus_nbar; j += 8) 
        {
            OP_clear_8x8(&R_acc);
            for (k = 0; k < scloudplus_n; k += 8) 
            {
                OP_load_8x8_safe(&R_c, C, i, k, scloudplus_mbar, scloudplus_n);
                OP_load_transposed_8x8_safe(&R_s, S, j, k, scloudplus_nbar, scloudplus_n);
                OP_matrix_mul_8x8(R_res.val, R_c.val, R_s.val, 0); 
                OP_add_8x8(&R_acc, &R_res);
            }
            OP_store_accumulate_8x8_safe(out, &R_acc, i, j, scloudplus_mbar, scloudplus_nbar);
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
	int i, j, k;
    matrix8x8_t R_s, R_b, R_res, R_acc;
	for (i = 0; i < scloudplus_mbar; i += 8) 
    {
        for (j = 0; j < scloudplus_nbar; j += 8) 
        {
            OP_clear_8x8(&R_acc);

            for (k = 0; k < scloudplus_m; k += 8) 
            {
                OP_load_8x8_safe(&R_s, S, i, k, scloudplus_mbar, scloudplus_m);
                OP_load_8x8_safe(&R_b, B, k, j, scloudplus_m, scloudplus_nbar);
                OP_matrix_mul_8x8(R_res.val, R_s.val, R_b.val, 0);
                OP_add_8x8(&R_acc, &R_res);
            }
            OP_store_accumulate_8x8_safe(out, &R_acc, i, j, scloudplus_mbar, scloudplus_nbar);
        }
    }
}
