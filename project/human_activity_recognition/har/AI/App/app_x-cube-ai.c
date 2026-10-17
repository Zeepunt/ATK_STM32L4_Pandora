
/**
  ******************************************************************************
  * @file    app_x-cube-ai.c
  * @author  X-CUBE-AI C code generator
  * @brief   AI program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

  /**
    * Description
    * v1.0: Minimum template to show how to use the Embedded Client API ST-AI 
    *
        */

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#if defined ( __ICCARM__ )
#define AI_RAM   _Pragma("location=\"AI_RAM\"")
#define AI_RAM2   _Pragma("location=\"AI_RAM2\"")
#elif defined ( __CC_ARM ) || ( __GNUC__ )
#define AI_RAM   __attribute__((section(".AI_RAM")))
#define AI_RAM2   __attribute__((section(".AI_RAM2")))
#endif

/* System headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "app_x-cube-ai.h"
#include "bsp_ai.h"

#include "stai.h"



/* USER CODE BEGIN includes */

/* USER CODE END includes */

/* IO buffers ----------------------------------------------------------------*/


/* Input defs ----------------------------------------------------------------*/
#include "aiTestUtility.h"
/**
STAI_ALIGNED(32) static uint8_t data_in_1[STAI_NETWORK_IN_1_SIZE_BYTES];

// Array to store the data of the input tensor
stai_ptr data_ins[] = {
  data_in_1
}; 
*/

/* Output defs ----------------------------------------------------------------*/

/**
STAI_ALIGNED(32) 
static uint8_t data_out_1[STAI_NETWORK_OUT_1_SIZE_BYTES];

// c-array to store the data of the output tensor
stai_ptr data_outs[] = {
  data_out_1
}; 
*/




/* Global byte buffer to save instantiated C-model network context */
STAI_NETWORK_CONTEXT_DECLARE(network_context, STAI_NETWORK_CONTEXT_SIZE)

/* Activations buffers -------------------------------------------------------*/
STAI_ALIGNED(32) 
AI_RAM 
static uint8_t RAM[STAI_NETWORK_ACTIVATION_1_SIZE_BYTES];


/* Global c-array to handle the activations buffer */
stai_ptr data_activations[] = { RAM };

STAI_ALIGNED(32) static uint8_t states_1[4];
stai_ptr data_states[] = {
    states_1
};




/* Entry points --------------------------------------------------------------*/

/* Array of pointer to manage the model's input/output tensors */
static stai_size in_length, out_length;
static stai_ptr stai_input[STAI_NETWORK_IN_NUM];
static stai_ptr stai_output[STAI_NETWORK_OUT_NUM];


/* 
 * Bootstrap
 */
int aiInit(void) {
  stai_return_code ret_code;
  
  /* 1: Initialize runtime library */
  ret_code = stai_runtime_init();
  
  /* 2: Initialize network model context */
  ret_code = user_stai_network_init(network_context);
  /* 3: Set network activations buffers */
  ret_code = stai_network_set_activations(network_context, data_activations, STAI_NETWORK_ACTIVATIONS_NUM);
  

  /* 4: Update the AI input/output buffers */
  /** Set network input/output buffers 
    * If the model uses no-inputs-allocation or no-outputs-allocation, the addresses of the input/output buffers
    * must be set before running the inference.
    * See https://stedgeai-dc.com/assets/embedded-docs/embedded_client_stai_api.html#ref_api_set_io
    * for more details
    */

  // current model uses allocate-inputs, use this part to overwrite the addresses of the input buffers
  /**
  ret_code = stai_network_set_inputs(network_context, data_ins, STAI_NETWORK_IN_NUM);
   */
  // current model uses allocate-outputs, use this part to overwrite the addresses of the output buffers
  /** 
  ret_code = stai_network_set_outputs(network_context, data_outs, STAI_NETWORK_OUT_NUM);
   */

  ret_code = stai_network_get_inputs(network_context, stai_input, &in_length);
  
  ret_code = stai_network_get_outputs(network_context, stai_output, &out_length);
  
  return 0;
}

int aiDeinit(void) {
  stai_return_code ret_code;

  /* 1: Deinitialize network model context */
  ret_code = stai_network_deinit(network_context);
  
  /* 2: Deinitialize runtime library */
  ret_code = stai_runtime_deinit();

  return 0;
}

/* 
 * Run inference
 */
stai_return_code aiRun() {
  stai_return_code ret_code;

  /** Profiling code to calculate the inference time of the model. You can remove it if not needed */
  static uint32_t inference_nb = 0;
  static uint32_t total_cycles = 0;
  uint32_t start_tick, end_tick, end_dwt = 0;
  struct dwtTime t;
  cyclesCounterInit();

  LC_PRINT("---- Inference number %" PRIu32 " ----\r\n", inference_nb);
  LC_PRINT("Results for network \"%s\"\r\nRunning...\r\n", STAI_NETWORK_MODEL_NAME);
  cyclesCounterStart();
  start_tick = HAL_GetTick();


  /* Perform the inference */
  ret_code = stai_network_run(network_context, STAI_MODE_SYNC);
  if (ret_code != STAI_SUCCESS) {
      ret_code = stai_network_get_error(network_context);
      LC_PRINT("Inference failed with error code %s\r\n", stai_get_return_code_name(ret_code));
  };
  /** End of inference */
  
  /** Continue profiling */
  end_dwt = cyclesCounterEnd();
  total_cycles += end_dwt;
  end_tick = HAL_GetTick();
  dwtCyclesToTime(end_dwt, &t);

  LC_PRINT(" duration DWT    : %d.%03d ms\r\n", t.s * 1000 + t.ms, t.us);
  LC_PRINT(" duration SysTick: %" PRIu32" ms\r\n", end_tick - start_tick);
  LC_PRINT(" CPU cycles      : %" PRIu32 "\r\n", end_dwt);
  LC_PRINT(" CPU cycles (avg): %" PRIu32 "\r\n", total_cycles / ++inference_nb);
  LC_PRINT(" Sleep for 5s...\r\n");
  HAL_Delay(5000);

  return ret_code;
}


#include <math.h>

int acquire_and_process_data()
{
    #define WINDOWS_LENGTH 24
    #define ACCEL_NUM      3

    int16_t ax = 0;
    int16_t ay = 0;
    int16_t az = 0;

    float *in = (float *)stai_input[0];

    float window[WINDOWS_LENGTH][ACCEL_NUM];

    /* 采样 24 个点 */
    for (int i = 0; i < WINDOWS_LENGTH; i++) {
        extern int dev_icm20608_accelerometer_get(int16_t *x, int16_t *y, int16_t *z);
        dev_icm20608_accelerometer_get(&ax, &ay, &az);

        /* 加速度计 FSR = ±2g -> 16384 LSB/g */
        window[i][0] = (float)ax / 16384.0f;
        window[i][1] = (float)ay / 16384.0f;
        window[i][2] = (float)az / 16384.0f;

        /* 1000ms / 26 Hz ≈ 38.5 ms */
        HAL_Delay(38);
    }

    /* 计算窗口的平均重力向量 */
    float gx = 0.0f, gy = 0.0f, gz = 0.0f;
    for (int i = 0; i < WINDOWS_LENGTH; i++) {
        gx += window[i][0];
        gy += window[i][1];
        gz += window[i][2];
    }
    gx /= WINDOWS_LENGTH;
    gy /= WINDOWS_LENGTH;
    gz /= WINDOWS_LENGTH;
    float g_norm = sqrtf(gx * gx + gy * gy + gz * gz);

    /* 构造旋转矩阵 R (Rodrigues), 把重力向量对齐到 +Z 轴 */
    float R[3][3];

    if (g_norm > 0.1f) {
        float ux = gx / g_norm;
        float uy = gy / g_norm;
        float uz = gz / g_norm;

        /* 旋转轴 = 重力向量 × Z 轴 */
        float axis_x =  uy;
        float axis_y = -ux;
        float axis_z =  0.0f;
        float axis_norm = sqrtf(axis_x * axis_x + axis_y * axis_y);

        /* 旋转角 = 重力与 Z 轴夹角 */
        float angle = acosf(uz);

        if (axis_norm > 1e-6f) {
            axis_x /= axis_norm;
            axis_y /= axis_norm;

            float c = cosf(angle);
            float s = sinf(angle);
            float t = 1.0f - c;

            R[0][0] = t * axis_x * axis_x + c;
            R[0][1] = t * axis_x * axis_y - s * axis_z;
            R[0][2] = t * axis_x * axis_z + s * axis_y;

            R[1][0] = t * axis_x * axis_y + s * axis_z;
            R[1][1] = t * axis_y * axis_y + c;
            R[1][2] = t * axis_y * axis_z - s * axis_x;

            R[2][0] = t * axis_x * axis_z - s * axis_y;
            R[2][1] = t * axis_y * axis_z + s * axis_x;
            R[2][2] = t * axis_z * axis_z + c;
        } else {
            /* 重力已沿 ±Z, 退化为单位矩阵或 Z 翻转 */
            memset(R, 0, sizeof(R));
            R[0][0] = 1.0f;
            R[1][1] = 1.0f;
            R[2][2] = (uz > 0.0f) ? 1.0f : -1.0f;
        }
    } else {
        /* 重力接近 0 (自由落体或剧烈运动), 不旋转 */
        memset(R, 0, sizeof(R));
        R[0][0] = 1.0f;
        R[1][1] = 1.0f;
        R[2][2] = 1.0f;
    }

    /* 逐点旋转 + 重力抑制, 并写入 STAI 输入张量 */
    for (int i = 0; i < WINDOWS_LENGTH; i++) {
        float x = window[i][0];
        float y = window[i][1];
        float z = window[i][2];

        float rx = R[0][0] * x + R[0][1] * y + R[0][2] * z;
        float ry = R[1][0] * x + R[1][1] * y + R[1][2] * z;
        float rz = R[2][0] * x + R[2][1] * y + R[2][2] * z;

        in[i * ACCEL_NUM + 0] = rx;
        in[i * ACCEL_NUM + 1] = ry;
        in[i * ACCEL_NUM + 2] = rz - g_norm;   /* 减去重力分量 */
    }

    return 0;
}

int post_process()
{
    #define LABELS_NUM    4
    static const char *labels[LABELS_NUM] = {
        "Stationary", /* 静止 */
        "Walking",    /* 步行 */
        "Jogging",    /* 慢跑 */
        "Biking",     /* 骑行 */
    };

    const float *out = (const float *)stai_output[0];

    int best_class = 0;
    float best_conf = out[0];

    for (int i = 1; i < LABELS_NUM; i++) {
        if (out[i] > best_conf) {
            best_conf = out[i];
            best_class = i;
        }
    }

    printf("---- post_process ----\r\n");
    printf("Predicted: %s (conf=%.3f)\r\n", labels[best_class], best_conf);
    for (int i = 0; i < LABELS_NUM; i++) {
        printf("  %s: %.3f\r\n", labels[i], out[i]);
    }

    return 0;
}



/* 
 * Example of main loop function
 */
void main_loop() {
  /* USER CODE BEGIN main_loop */
  while (1) {
    /* 1 - Acquire, pre-process and fill the input buffers */
    acquire_and_process_data();

    /* 2 - Call inference engine */
    aiRun();

    /* 3 - Post-process the predictions */
    post_process();
  }
  /* USER CODE END main_loop */
}


/* Entry points --------------------------------------------------------------*/


void STM32CubeAI_Studio_AI_Init(void)
{
    aiInit();  
    /* USER CODE BEGIN init */
    /* USER CODE END init */
}

void STM32CubeAI_Studio_AI_Process(void)
{
    main_loop();
} 

void STM32CubeAI_Studio_AI_Deinit(void)
{
    aiDeinit();
} 


#ifdef __cplusplus
}
#endif
