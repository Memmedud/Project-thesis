/*
 * Copyright (C) 2010-2021 Arm Limited or its affiliates.
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
 *
 * Modifications copyright (C) 2021-2022 Chair of Electronic Design Automation, TUM
 */

#include <muriscv_nn_functions.h>
#include <stdlib.h>
#include <unity.h>

#include "../../TestData/fc_int16_slow/test_data.h"
#include "../../TestData/fully_connected_int16/test_data.h"
#include "../../TestData/fully_connected_int16_big/test_data.h"
#include "../../Utils/validate.h"

void setUp(void)
{ /* set stuff up here */
}

void tearDown(void)
{ /* clean stuff up here */
}

void fully_connected_int16_muriscv_nn_fully_connected_s16(void)
{
    const muriscv_nn_status expected = MURISCV_NN_SUCCESS;
    q15_t output[FULLY_CONNECTED_INT16_DST_SIZE] = {0};

    muriscv_nn_context ctx;
    muriscv_nn_fc_params fc_params;
    muriscv_nn_per_tensor_quant_params quant_params;
    muriscv_nn_dims input_dims;
    muriscv_nn_dims filter_dims;
    muriscv_nn_dims bias_dims;
    muriscv_nn_dims output_dims;

    const q63_t *bias_data = fully_connected_int16_biases;
    const q7_t *kernel_data = fully_connected_int16_weights;
    const q15_t *input_data = fully_connected_int16_input;
    const q15_t *output_ref = fully_connected_int16_output_ref;
    const int32_t output_ref_size = FULLY_CONNECTED_INT16_DST_SIZE;

    input_dims.n = FULLY_CONNECTED_INT16_INPUT_BATCHES;
    input_dims.w = FULLY_CONNECTED_INT16_INPUT_W;
    input_dims.h = FULLY_CONNECTED_INT16_INPUT_H;
    input_dims.c = FULLY_CONNECTED_INT16_IN_CH;
    filter_dims.n = FULLY_CONNECTED_INT16_ACCUMULATION_DEPTH;
    filter_dims.c = FULLY_CONNECTED_INT16_OUT_CH;
    filter_dims.h = FULLY_CONNECTED_INT16_INPUT_H;
    filter_dims.w = FULLY_CONNECTED_INT16_INPUT_W;
    output_dims.n = FULLY_CONNECTED_INT16_INPUT_BATCHES;
    output_dims.c = FULLY_CONNECTED_INT16_OUT_CH;

    fc_params.input_offset = 0;
    fc_params.filter_offset = 0;
    fc_params.output_offset = 0;
    fc_params.activation.min = FULLY_CONNECTED_INT16_OUT_ACTIVATION_MIN;
    fc_params.activation.max = FULLY_CONNECTED_INT16_OUT_ACTIVATION_MAX;

    quant_params.multiplier = FULLY_CONNECTED_INT16_OUTPUT_MULTIPLIER;
    quant_params.shift = FULLY_CONNECTED_INT16_OUTPUT_SHIFT;

    int32_t buf_size = muriscv_nn_fully_connected_s16_get_buffer_size(&filter_dims);
    ctx.buf = malloc(buf_size);
    ctx.size = buf_size;

    muriscv_nn_status result = muriscv_nn_fully_connected_s16(&ctx,
                                                              &fc_params,
                                                              &quant_params,
                                                              &input_dims,
                                                              input_data,
                                                              &filter_dims,
                                                              kernel_data,
                                                              &bias_dims,
                                                              bias_data,
                                                              &output_dims,
                                                              output);

    free(ctx.buf);
    TEST_ASSERT_EQUAL(expected, result);
    TEST_ASSERT_TRUE(validate_s16(output, output_ref, output_ref_size));
}

void fully_connected_int16_big_muriscv_nn_fully_connected_s16(void)
{
    const muriscv_nn_status expected = MURISCV_NN_SUCCESS;
    q15_t output[FULLY_CONNECTED_INT16_BIG_DST_SIZE] = {0};

    muriscv_nn_context ctx;
    muriscv_nn_fc_params fc_params;
    muriscv_nn_per_tensor_quant_params quant_params;
    muriscv_nn_dims input_dims;
    muriscv_nn_dims filter_dims;
    muriscv_nn_dims bias_dims;
    muriscv_nn_dims output_dims;

    const q63_t *bias_data = fully_connected_int16_big_biases;
    const q7_t *kernel_data = fully_connected_int16_big_weights;
    const q15_t *input_data = fully_connected_int16_big_input;
    const q15_t *output_ref = fully_connected_int16_big_output_ref;
    const int32_t output_ref_size = FULLY_CONNECTED_INT16_BIG_DST_SIZE;

    input_dims.n = FULLY_CONNECTED_INT16_BIG_INPUT_BATCHES;
    input_dims.w = FULLY_CONNECTED_INT16_BIG_INPUT_W;
    input_dims.h = FULLY_CONNECTED_INT16_BIG_INPUT_H;
    input_dims.c = FULLY_CONNECTED_INT16_BIG_IN_CH;
    filter_dims.n = FULLY_CONNECTED_INT16_BIG_ACCUMULATION_DEPTH;
    filter_dims.c = FULLY_CONNECTED_INT16_BIG_OUT_CH;
    filter_dims.h = FULLY_CONNECTED_INT16_BIG_INPUT_H;
    filter_dims.w = FULLY_CONNECTED_INT16_BIG_INPUT_W;
    output_dims.n = FULLY_CONNECTED_INT16_BIG_INPUT_BATCHES;
    output_dims.c = FULLY_CONNECTED_INT16_BIG_OUT_CH;

    fc_params.input_offset = 0;
    fc_params.filter_offset = 0;
    fc_params.output_offset = 0;
    fc_params.activation.min = FULLY_CONNECTED_INT16_BIG_OUT_ACTIVATION_MIN;
    fc_params.activation.max = FULLY_CONNECTED_INT16_BIG_OUT_ACTIVATION_MAX;

    quant_params.multiplier = FULLY_CONNECTED_INT16_BIG_OUTPUT_MULTIPLIER;
    quant_params.shift = FULLY_CONNECTED_INT16_BIG_OUTPUT_SHIFT;

    int32_t buf_size = muriscv_nn_fully_connected_s16_get_buffer_size(&filter_dims);
    ctx.buf = malloc(buf_size);
    ctx.size = buf_size;

    muriscv_nn_status result = muriscv_nn_fully_connected_s16(&ctx,
                                                              &fc_params,
                                                              &quant_params,
                                                              &input_dims,
                                                              input_data,
                                                              &filter_dims,
                                                              kernel_data,
                                                              &bias_dims,
                                                              bias_data,
                                                              &output_dims,
                                                              output);

    free(ctx.buf);
    TEST_ASSERT_EQUAL(expected, result);
    TEST_ASSERT_TRUE(validate_s16(output, output_ref, output_ref_size));
}

void fc_int16_slow_muriscv_nn_fully_connected_s16(void)
{
    const muriscv_nn_status expected = MURISCV_NN_SUCCESS;
    q15_t output[FC_INT16_SLOW_DST_SIZE] = {0};

    muriscv_nn_context ctx;
    muriscv_nn_fc_params fc_params;
    muriscv_nn_per_tensor_quant_params quant_params;
    muriscv_nn_dims input_dims;
    muriscv_nn_dims filter_dims;
    muriscv_nn_dims bias_dims;
    muriscv_nn_dims output_dims;

    const q63_t *bias_data = fc_int16_slow_biases;
    const q7_t *kernel_data = fc_int16_slow_weights;
    const q15_t *input_data = fc_int16_slow_input;
    const q15_t *output_ref = fc_int16_slow_output_ref;
    const int32_t output_ref_size = FC_INT16_SLOW_DST_SIZE;

    input_dims.n = FC_INT16_SLOW_INPUT_BATCHES;
    input_dims.w = FC_INT16_SLOW_INPUT_W;
    input_dims.h = FC_INT16_SLOW_INPUT_H;
    input_dims.c = FC_INT16_SLOW_IN_CH;
    filter_dims.n = FC_INT16_SLOW_ACCUMULATION_DEPTH;
    filter_dims.c = FC_INT16_SLOW_OUT_CH;
    filter_dims.h = FC_INT16_SLOW_INPUT_H;
    filter_dims.w = FC_INT16_SLOW_INPUT_W;
    output_dims.n = FC_INT16_SLOW_INPUT_BATCHES;
    output_dims.c = FC_INT16_SLOW_OUT_CH;

    fc_params.input_offset = 0;
    fc_params.filter_offset = 0;
    fc_params.output_offset = 0;
    fc_params.activation.min = FC_INT16_SLOW_OUT_ACTIVATION_MIN;
    fc_params.activation.max = FC_INT16_SLOW_OUT_ACTIVATION_MAX;

    quant_params.multiplier = FC_INT16_SLOW_OUTPUT_MULTIPLIER;
    quant_params.shift = FC_INT16_SLOW_OUTPUT_SHIFT;

    int32_t buf_size = muriscv_nn_fully_connected_s16_get_buffer_size(&filter_dims);
    ctx.buf = malloc(buf_size);
    ctx.size = buf_size;

    muriscv_nn_status result = muriscv_nn_fully_connected_s16(&ctx,
                                                              &fc_params,
                                                              &quant_params,
                                                              &input_dims,
                                                              input_data,
                                                              &filter_dims,
                                                              kernel_data,
                                                              &bias_dims,
                                                              bias_data,
                                                              &output_dims,
                                                              output);

    free(ctx.buf);
    TEST_ASSERT_EQUAL(expected, result);
    TEST_ASSERT_TRUE(validate_s16(output, output_ref, output_ref_size));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(fully_connected_int16_muriscv_nn_fully_connected_s16);
    RUN_TEST(fully_connected_int16_big_muriscv_nn_fully_connected_s16);
    RUN_TEST(fc_int16_slow_muriscv_nn_fully_connected_s16);

#if defined(__riscv) || defined(__riscv__)
    /* If an error occurred make sure the simulator fails so CTest can detect that. */
    int failures = UNITY_END();
    if (failures != 0)
    {
        __asm__ volatile("unimp");
    }
    return failures;
#else
    return UNITY_END();
#endif
}
