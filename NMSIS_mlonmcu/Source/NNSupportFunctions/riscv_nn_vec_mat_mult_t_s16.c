/*
 * SPDX-FileCopyrightText: Copyright 2020-2023 Arm Limited and/or its affiliates <open-source-office@arm.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ----------------------------------------------------------------------
 * Project:      NMSIS NN Library
 * Title:        riscv_nn_vec_mat_mult_t_s16
 * Description:  s16 vector by matrix (transposed) multiplication
 *
 * $Date:        5 January 2023
 * $Revision:    V.2.2.0
 *
 * Target Processor: RISC-V Cores
 *
 * -------------------------------------------------------------------- */

#include "riscv_nnsupportfunctions.h"

#define MAX_COL_COUNT (512)

/**
 * @ingroup groupSupport
 */

/**
 * @addtogroup supportFC
 * @{
 */

/*
 * s16 vector(lhs) by matrix (transposed) multiplication
 *
 * Refer header file for details.
 *
 */
riscv_nmsis_nn_status riscv_nn_vec_mat_mult_t_s16(const int16_t *lhs,
                                              const int8_t *rhs,
                                              const int64_t *bias,
                                              int16_t *dst,
                                              const int32_t dst_multiplier,
                                              const int32_t dst_shift,
                                              const int32_t rhs_cols,
                                              const int32_t rhs_rows,
                                              const int32_t activation_min,
                                              const int32_t activation_max)
{
#if defined(RISCV_MATH_DSP)

    int32_t rhs_cols_fast = rhs_cols;

    if (rhs_cols > MAX_COL_COUNT)
    {
        rhs_cols_fast = MAX_COL_COUNT;
    }

    const int32_t row_loop_cnt = rhs_rows / 2;

    for (int32_t i = 0; i < row_loop_cnt; i++)
    {

        int64_t acc_64_0 = 0;
        int64_t acc_64_1 = 0;
        int32_t acc_0 = 0;
        int32_t acc_1 = 0;

        const int32_t col_loop_cnt = rhs_cols_fast / 4;

        const int16_t *lhs_vec = lhs;
        const int8_t *rhs_0 = rhs;
        const int8_t *rhs_1 = rhs + rhs_cols;
        rhs += 2 * rhs_cols;

        for (int j = col_loop_cnt; j != 0; j--)
        {
            int32_t ker_0, ker_1, vec_part_0, vec_part_1;

            vec_part_0 = riscv_nn_read_q15x2_ia(&lhs_vec);
            vec_part_1 = riscv_nn_read_q15x2_ia(&lhs_vec);

            rhs_0 = read_and_pad(rhs_0, &ker_0, &ker_1);

            acc_0 = __SMLAD(ker_0, vec_part_0, acc_0);
            acc_0 = __SMLAD(ker_1, vec_part_1, acc_0);

            rhs_1 = read_and_pad(rhs_1, &ker_0, &ker_1);

            acc_1 = __SMLAD(ker_0, vec_part_0, acc_1);
            acc_1 = __SMLAD(ker_1, vec_part_1, acc_1);
        }

        acc_64_0 += acc_0;
        acc_64_1 += acc_1;

        for (int k = col_loop_cnt * 4; k < rhs_cols; k++)
        {
            const int32_t lhs_temp = (*lhs_vec);
            lhs_vec++;
            acc_64_0 += lhs_temp * (*rhs_0);
            rhs_0++;
            acc_64_1 += lhs_temp * (*rhs_1);
            rhs_1++;
        }

        if (bias)
        {
            acc_64_0 += *bias++;
            acc_64_1 += *bias++;
        }
        int32_t tmp;

        tmp = riscv_nn_requantize_s64(acc_64_0, dst_multiplier, dst_shift);
        tmp = MAX(tmp, activation_min);
        tmp = MIN(tmp, activation_max);
        *dst++ = (int16_t)tmp;

        tmp = riscv_nn_requantize_s64(acc_64_1, dst_multiplier, dst_shift);
        tmp = MAX(tmp, activation_min);
        tmp = MIN(tmp, activation_max);
        *dst++ = (int16_t)tmp;
    }

    if (rhs_rows & 0x1)
    {
        int64_t acc_64_0 = 0;
        int32_t acc_0 = 0;
        const int32_t col_loop_cnt = rhs_cols_fast / 4;

        const int16_t *lhs_vec = lhs;
        const int8_t *rhs_0 = rhs;

        for (int i = col_loop_cnt; i != 0; i--)
        {
            int32_t ker_0, ker_1, vec;
            rhs_0 = read_and_pad(rhs_0, &ker_0, &ker_1);

            vec = riscv_nn_read_q15x2_ia(&lhs_vec);
            acc_0 = __SMLAD(ker_0, vec, acc_0);

            vec = riscv_nn_read_q15x2_ia(&lhs_vec);
            acc_0 = __SMLAD(ker_1, vec, acc_0);
        }

        acc_64_0 += acc_0;

        for (int j = col_loop_cnt * 4; j < rhs_cols; j++)
        {
            const int32_t lhs_temp = (*lhs_vec);
            lhs_vec++;
            acc_64_0 += lhs_temp * (*rhs_0);
            rhs_0++;
        }

        if (bias)
        {
            acc_64_0 += *bias++;
        }
        int32_t tmp;
        tmp = riscv_nn_requantize_s64(acc_64_0, dst_multiplier, dst_shift);
        tmp = MAX(tmp, activation_min);
        tmp = MIN(tmp, activation_max);
        *dst++ = (int16_t)tmp;
    }

#else
    for (int i_row_loop_cnt = 0; i_row_loop_cnt < rhs_rows; i_row_loop_cnt++)
    {
        const int16_t *lhs_ptr = lhs;
        const int8_t *rhs_ptr_0 = &rhs[0];

        int64_t result = 0;

        for (int32_t rhs_cols_idx = 0; rhs_cols_idx < rhs_cols; ++rhs_cols_idx)
        {
            const int64_t rhs_value0 = (int8_t)*rhs_ptr_0;
            const int64_t lhs_value = *lhs_ptr;

            result += lhs_value * rhs_value0;

            ++rhs_ptr_0;
            ++lhs_ptr;
        }

        if (bias)
        {
            result += *bias++;
        }
        // Quantize down
        result = riscv_nn_requantize_s64(result, dst_multiplier, dst_shift);

        // Clamp the result
        result = ((result) > (activation_min) ? (result) : (activation_min));
        result = ((result) < (activation_max) ? (result) : (activation_max));

        *dst++ = (int16_t)result;
        rhs += rhs_cols;
    }
#endif

    return RISCV_NMSIS_NN_SUCCESS;
}

/**
 * @} end of Doxygen group
 */
