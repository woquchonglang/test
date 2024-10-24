#include "lpf_filter.h"
#define false 0
#define true 1

#include "math.h"
#define PI 3.141f
// 低通滤波器
void LPF_init(LPF_t *filter, float ts, float fc) {
  filter->orig_val = 0;
  filter->fltr_val = 0;
  filter->ts = ts;
  filter->fc = fc;
  filter->a = filter->ts / (filter->ts + 1.0f / 2.f / PI / filter->fc);
}

float LPF_update(LPF_t *filter, float new_value) {

  filter->a = filter->ts / (filter->ts + 1.0f / 2.f / PI / filter->fc);
  if (isnan(new_value) == false)
    filter->orig_val = new_value;
  else
    return filter->fltr_val;

  if (isnan(filter->fltr_val) == false) {
    filter->fltr_val = filter->fltr_val +
                       filter->a * (filter->orig_val -
                                    filter->fltr_val); // 一阶低通滤波差分方程
  } else {
    filter->fltr_val = filter->orig_val;
  }

  return filter->fltr_val;
}

// 初始化滤波器
// void ButterworthLPF_init(ButterworthLPF_t *filter, float cutoff_freq,
//                          float sample_rate) {
//   float omega = 2.0f * M_PI * cutoff_freq / sample_rate; // 归一化频率
//   float omega2 = omega * omega;
//   float sqrt2 = sqrt(2.0f); // 根号2

//   // 计算系数
//   float k = 1.0f / (1.0f + sqrt2 * omega + omega2);
//   filter->a[0] = omega2 * k;
//   filter->a[1] = 2.0f * filter->a[0];
//   filter->a[2] = filter->a[0];
//   filter->b[1] = 2.0f * (1.0f - omega2) * k;
//   filter->b[2] = (1.0f - sqrt2 * omega + omega2) * k;

//   // 初始化输入和输出值为0
//   for (int i = 0; i < 3; i++) {
//     filter->x[i] = 0.0f;
//     filter->y[i] = 0.0f;
//   }
// }

void ButterworthLPF_init(ButterworthLPF *filter, float sample_rate, float cutoff_freq) {
    float omega = 2.0f * M_PI * cutoff_freq / sample_rate;
    float sn = sinf(omega);
    float cs = cosf(omega);
    float alpha = sn / (2.0f * sqrtf(2.0f)); // Q = sqrt(2)/2 for Butterworth

    float b0 = (1.0f - cs) / 2.0f;
    float b1 = 1.0f - cs;
    float b2 = (1.0f - cs) / 2.0f;
    float a0 = 1.0f + alpha;
    float a1 = -2.0f * cs;
    float a2 = 1.0f - alpha;

    filter->b[0] = b0 / a0;
    filter->b[1] = b1 / a0;
    filter->b[2] = b2 / a0;
    filter->a[0] = 1.0f; // a0 is normalized to 1
    filter->a[1] = a1 / a0;
    filter->a[2] = a2 / a0;

    filter->x[0] = filter->x[1] = 0.0f;
    filter->y[0] = filter->y[1] = 0.0f;
}

float ButterworthLPF_update(ButterworthLPF *filter, float input) {
  float output = filter->b[0] * input + filter->b[1] * filter->x[0] +
                 filter->b[2] * filter->x[1] - filter->a[1] * filter->y[0] -
                 filter->a[2] * filter->y[1];

  filter->x[1] = filter->x[0];
  filter->x[0] = input;
  filter->y[1] = filter->y[0];
  filter->y[0] = output;

  return output;
}

// // 更新滤波器
// float ButterworthLPF_update(ButterworthLPF_t *filter, float new_value) {
//   // 将输入值存储
//   filter->x[0] = new_value;

//   // 计算输出值
//   filter->y[0] = filter->a[0] * filter->x[0] + filter->a[1] * filter->x[1] +
//                  filter->a[2] * filter->x[2] - filter->b[1] * filter->y[1] -
//                  filter->b[2] * filter->y[2];

//   // 更新输入和输出历史值
//   filter->x[2] = filter->x[1];
//   filter->x[1] = filter->x[0];
//   filter->y[2] = filter->y[1];
//   filter->y[1] = filter->y[0];

//   return filter->y[0]; // 返回输出值
// }

void calculate_3rd_order_butterworth_lpf_coeffs(float cutoff_freq,
                                                float sample_rate, float *a,
                                                float *b) {
  float omega = 2 * M_PI * cutoff_freq / sample_rate;
  float alpha = sin(omega) / (2 * 1.414); // 1.414 是 sqrt(2)
  float beta = cos(omega);

  a[0] = 1.0;
  a[1] = 2 * (alpha * beta - 1);
  a[2] = 1 - alpha * beta - alpha * beta + 1;
  a[3] = a[2];

  b[0] = alpha * (1 + beta) / (1 + a[1] + a[2] + a[3]);
  b[1] = 2 * b[0];
  b[2] = b[0];
}
void ButterworthLPF_3_init(ButterworthLPF_3_t *filter, float cutoff_freq,
                           float sample_rate) {
  calculate_3rd_order_butterworth_lpf_coeffs(cutoff_freq, sample_rate,
                                             filter->a, filter->b);
  for (int i = 0; i < 4; ++i) {
    filter->x[i] = 0.0;
    filter->y[i] = 0.0;
  }
}

float ButterworthLPF_3_process(ButterworthLPF_3_t *filter, float input) {
  // Update the input and output buffers
  for (int i = 3; i > 0; --i) {
    filter->x[i] = filter->x[i - 1];
    filter->y[i] = filter->y[i - 1];
  }
  filter->x[0] = input;

  // Compute the output
  float output = 0.0;
  for (int i = 0; i < 4; ++i) {
    output += filter->a[i] * filter->x[i] - filter->b[i] * filter->y[i];
  }

  // Update the output buffer
  filter->y[0] = output;

  return output;
}
