/*
 * @Author: yjy 3253932102@qq.com
 * @Date: 2024-10-22 16:28:36
 * @LastEditors: yjy 3253932102@qq.com
 * @LastEditTime: 2024-10-22 18:17:15
 * @FilePath: /go_test/robot/tool/vofa.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef VOFA_H
#define VOFA_H


#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

typedef enum {
  LIFT_VOFA = 0,
  EXTEND_VOFA,
  PITCH_VOFA,
  CHASSIS_VOFA,
  ROLL_VOFA,
  SPIN_VOFA,
  PTZ_VOFA,
  YAW_VOFA,
  TEST,
  CC_CTRL,
  CC_ARM,
} vofa_t;

void show_data_to_vofa(void);

void host_init(void);

int _printf(const char *format, ...);

void V0fa_FireWater(const char *format, ...);
void V0fa_JustFloat(float *_data, uint8_t _num);

#ifdef __cplusplus
}
#endif

#endif
