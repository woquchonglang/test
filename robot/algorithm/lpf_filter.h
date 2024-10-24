/*
 * @Author: TAN7_Main 1240973173@qq.com
 * @Date: 2023-11-28 23:09:44
 * @LastEditors: yjy 3253932102@qq.com
 * @LastEditTime: 2024-10-24 15:47:32
 * @FilePath: \24_Engineer\algorithm\lpf_filter\lpf_filter.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _Lpf_Filter_H_
#define _Lpf_Filter_H_
#include "main.h"
#include "util.h"
#define SMAPLE_LS_TASKS_TICK (5) // 2ms
// 低通滤波器
typedef struct
{
	float orig_val; // orignal value,原始值
	float fltr_val; // filter value,滤波值
	float ts;		// 采用间隔时间
	float fc;		// 截止频率
	float a;		// 比例系数,init会自动更新
} LPF_t;
void LPF_init(LPF_t *filter, float ts, float fc);
float LPF_update(LPF_t *filter, float new_value);

typedef struct {
    float a[3]; // 系数
    float b[3]; // 系数
    float x[3]; // 输入值
    float y[3]; // 输出值
} ButterworthLPF_t;

typedef struct {
  float a[3]; // 分母系数
  float b[3]; // 分子系数
  float x[2]; // 输入信号延迟
  float y[2]; // 输出信号延迟
} ButterworthLPF;

typedef struct {
  float x[4]; // Input buffer
  float y[4]; // Output buffer
  float a[4]; // Coefficients a
  float b[4]; // Coefficients b
} ButterworthLPF_3_t;

// void ButterworthLPF_init(ButterworthLPF_t *filter, float cutoff_freq, float sample_rate);
// float ButterworthLPF_update(ButterworthLPF_t *filter, float new_value);

void ButterworthLPF_init(ButterworthLPF *filter, float sample_rate, float cutoff_freq);
float ButterworthLPF_update(ButterworthLPF *filter, float input);


#endif
