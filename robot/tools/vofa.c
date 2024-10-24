#include "vofa.h"
#include "stdarg.h"
#include "stdint.h"
#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
#include "string.h"
#include <stdint.h>

extern UART_HandleTypeDef huart5;
extern DMA_HandleTypeDef hdma_usart5_tx;
#define UART_VOFA huart5

// #define MAX_FLOATS_NUM 8 // 最大浮点数数量
// #define buf_len ((MAX_FLOATS_NUM * (6 + 5 + 1) + 1))
// // 发送精度最大小数点�?5�?+小数点后5�?

// char buffer[buf_len] = {0}; // 选择足�?�大的缓冲区
// uint16_t tx_data_len = 0;
// struct {
//   float MAX_TX_FREQ;
//   float MIN_TX_TICK;
//   float REAL_TX_FREQ;
//   long Buffer_Boom_cnt; // 专门用于判断Buffer有没有炸掉，并�?�录炸掉的�?�数
// } VOFA_INFO = {0};
// float dfloat[8] = {0};

// /********************/
// #define _VOFA 1

// #define TEST_VOFA 1
// #define LIFT_VOFA 2


// /********************/

// extern float butter_tmp1;

//     // extern int pc9;

// void show_data_to_vofa(void) {
//   static uint32_t tickstart = 0;
//   if (/*host_uart.gState == HAL_UART_STATE_READY &&
//       HAL_GetTick() - tickstart > VOFA_INFO.MIN_TX_TICK +*/
//       1) {
//     VOFA_INFO.REAL_TX_FREQ = 1000.f / (HAL_GetTick() - tickstart);

//     // float Dfloats[MAX_FLOATS_NUM] = {0};
// #if (_VOFA == TEST_VOFA)
//     float Dfloats[MAX_FLOATS_NUM] = {
//         (float)butter_tmp1
//     };

// #elif (_VOFA == EXTEND_VOFA)

//     float Dfloats[MAX_FLOATS_NUM] = {
//         (float)lfet4.motor.extend.parsed.sum_path,
//         (float)lfet4.motor.extend.fbk.speed_rpm,
//         (float)lfet4.motor.extend.ctrl.rc_output,
//         (float)lfet4.tx.D[1],
//         (float)lfet4.motor.extend.parsed.filter_speed,
//         (float)lfet4.motor.extend.ctrl.pos,
//         (float)lfet4.motor.extend.pid.dual_pid[0].p_out,
//         (float)arm2.pitch.pid.dual_pid[1].p_out,
//     };

// #elif (_VOFA == PITCH_VOFA)

//     float Dfloats[MAX_FLOATS_NUM] = {
//         (float)arm2.pitch.recv.pos,
//         (float)arm2.pitch.recv.position,
//         (float)arm2.pitch.recv.velocity,
//         (float)arm2.pitch.recv.spd,
//         (float)arm2.pitch.filter_speed,
//         (float)arm2.pitch.ctrl.output,
//         (float)arm2.pitch.send.T_ff,
//         (float)0,
//         (float)0,
//         (float)0,
//         (float)0,
//     };

// #elif (_VOFA == YAW_VOFA)

//     float Dfloats[MAX_FLOATS_NUM] = {
//         (float)arm2.yaw.recv.pos,
//         (float)arm2.yaw.recv.W,
//         (float)arm2.yaw.ctrl.rc_output,
//         (float)arm2.yaw.send.T,
//         (float)arm2.yaw.filter_speed,
//         (float)arm2.yaw.ctrl.output,
//         (float)arm2.yaw.pid.dual_pid[0].p_out,
//         (float)arm2.yaw.pid.dual_pid[1].p_out,
//     };

// #elif (_VOFA == ROLL_VOFA)

//     float Dfloats[MAX_FLOATS_NUM] = {
//         (float)joint3.motor.roll.parsed.sum_path,
//         (float)joint3.motor.roll.fbk.speed_rpm,
//         (float)joint3.motor.roll.ctrl.rc_output,
//         (float)joint3.tx.D[2],
//         (float)joint3.motor.roll.parsed.filter_speed,
//         (float)joint3.motor.roll.ctrl.pos,
//         (float)joint3.motor.roll.pid.dual_pid[0].p_out,
//         (float)arm2.pitch.pid.dual_pid[1].p_out,
//     };

// #elif (_VOFA == SPIN_VOFA)

//     float Dfloats[MAX_FLOATS_NUM] = {
//         (float)-joint3.motor.spin1.parsed.sum_path,
//         (float)joint3.motor.spin2.parsed.sum_path,
//         (float)joint3.macro_ctrl.pitch_pos,
//         (float)-joint3.motor.spin1.ctrl.pos,
//         (float)joint3.motor.spin2.ctrl.pos,
//         (float)joint3.motor.spin1.ctrl.output,
//         (float)joint3.motor.spin2.ctrl.output,
//         (float)arm2.pitch.pid.dual_pid[1].p_out,
//     };

// #elif (_VOFA == TEST)

//     static float a = 0;
//     // a += 0.11f;
//     float Dfloats[MAX_FLOATS_NUM] = {
//         (float)arm2.pitch.send.K_P,
//         (float)arm2.yaw.send.K_P,
//         (float)30,
//         (float)40,
//         (float)50,
//         (float)cac++,
//         (float)a,
//         (float)a + 100,
//     };

// #elif _VOFA == CC_CTRL

//     float Dfloats[MAX_FLOATS_NUM] = {
//         (float)cus_dt_t.raw_dt.eular.pitch, (float)cus_dt_t.raw_dt.eular.yaw,
//         (float)cus_dt_t.raw_dt.eular.roll,  (float)p_data.data.eular.pitch,
//         (float)p_data.data.eular.yaw,       (float)p_data.data.eular.roll,
//         (float)cus_dt_t.filter_dt.vy,       (float)cus_dt_t.filter_dt.vx,
//     };

// #elif _VOFA == CC_ARM

//     float Dfloats[MAX_FLOATS_NUM] = {
//         (float)cc_arm.pitch.send.pos,
//         (float)cc_arm.yaw.send.pos,
//         (float)cc_arm.last_pitch.pos,
//         (float)cc_arm.last_yaw.pos,
//         (float)cus_dt_t.filter_dt.eular.pitch,
//         (float)cus_dt_t.filter_dt.eular.yaw,
//         (float)cus_dt_t.filter_dt.eular.roll,
//         (float)0,
//     };

// #elif _VOFA == CC_JOINT
//     float Dfloats[MAX_FLOATS_NUM] = {
//         (float)cc_joint.motor.spin1.ctrl.output,
//         (float)cc_joint.motor.spin2.ctrl.output,
//         (float)cc_joint.motor.roll.ctrl.output,
//         (float)cc_arm.last_yaw.pos,
//         (float)cus_dt_t.filter_dt.eular.pitch,
//         (float)cus_dt_t.filter_dt.eular.yaw,
//         (float)cus_dt_t.filter_dt.eular.roll,
//         (float)100,
//     };
// #elif _VOFA == CC_SM
//     float Dfloats[MAX_FLOATS_NUM] = {
//         (float)cc_arm.pitch.send.K_P,
//         (float)cc_arm.yaw.send.K_P,
//         (float)cc_arm.pitch.ctrl.pos,
//         (float)cc_arm.yaw.ctrl.pos,
//         (float)cc_joint.macro_ctrl.pitch_pos,
//         (float)cc_joint.macro_ctrl.ore_pos,
//         (float)cc_joint.macro_ctrl.roll_pos,
//         (float)0,
//     };

// #elif _VOFA == TEST_VIR_PATH
//     float Dfloats[MAX_FLOATS_NUM] = {
//         (float)move_point[0], (float)move_point[1], (float)move_point[2],
//         (float)move_point[3], (float)move_point[4], (float)move_point[5],
//         (float)move_point[6], (float)move_time[0],  (float)result_t,
//     };

// #elif _VOFA == TEST_REA_PATH
//     float Dfloats[MAX_FLOATS_NUM] = {
//         (float)RAW_PITCH,
//         (float)RAW_YAW,
//         (float)RAW_ROLL,
//         (float)lfet4.motor.ore.ctrl.pos,
//         (float)arm2.pitch.ctrl.pos,
//         (float)FIL_PITCH,
//         (float)FIL_YAW,
//         (float)FIL_ROLL,
//         //            (float)joint3.macro_ctrl.roll_pos,

//         (float)base4.motor.storage.parsed.sum_path,
//         (float)base4.motor.base_lift.parsed.sum_path,
//         (float)lfet4.motor.extend.parsed.sum_path,
//         (float)lfet4.motor.ore.recv.position,
//         (float)lfet4.motor.ore_extend.parsed.sum_path,
//         (float)arm2.pitch.ctrl.rc_output,
//         (float)arm2.yaw.ctrl.rc_output,
//         (float)joint3.macro_data.roll_pos,
//         (float)joint3.macro_data.ore_pos,
//         (float)joint3.macro_data.pitch_pos,

//     };

// #elif _VOFA == PTZ_VOFA
//     float Dfloats[MAX_FLOATS_NUM] = {
//         (float)base4.tx.D[0],
//         (float)base4.motor.ptz_lift.pid.dual_pid[0].i_out,
//         (float)base4.motor.ptz_lift.pid.reset_pid.p_out,

//     };
// #endif

//     memcpy(dfloat, Dfloats, sizeof(Dfloats));
//     char *buffer_ptr = buffer;

//     tx_data_len = 0;
//     for (int i = 0; i < MAX_FLOATS_NUM; i++) {
//       tx_data_len +=
//           snprintf(&buffer_ptr[tx_data_len], sizeof(buffer) - tx_data_len,
//                    "%.3f,", Dfloats[i]);
//     }
//     // 添加 \r\n 并发送数�?
//     if (tx_data_len > 0) {
//       buffer[tx_data_len - 1] = '\n';
//       HAL_UART_Transmit(&HOST_UART, (uint8_t *)buffer, tx_data_len, 0xfff);
//       // HAL_UART_Transmit(&HOST_UART, (uint8_t *)buffer, tx_data_len);
//     }
//     if (tx_data_len > buf_len) {
//       VOFA_INFO.Buffer_Boom_cnt++;
//     }

//     // 固定起�?�位1�?,数据�?8�?,无校验位,停�??�?1�?,数据量tx_data_len字节 �?
//     // 注意电机的current值是会大于四位数的，所以上面�?�使�?5�?
//     VOFA_INFO.MIN_TX_TICK =
//         1000.f * tx_data_len * (1 + 8 + 0 + 1) / HOST_UART.Init.BaudRate;
//     VOFA_INFO.MAX_TX_FREQ = 1000.f / VOFA_INFO.MIN_TX_TICK;
//     tickstart = HAL_GetTick();
//   }


  
// }


void V0fa_FireWater(const char *format, ...) {
  uint8_t txBuffer[100];
  uint32_t n;
  va_list args;
  va_start(args, format);
  n = vsnprintf((char *)txBuffer, 100, format, args);
      //....在此替换你的串口发送函数...........
      HAL_UART_Transmit(&UART_VOFA, (uint8_t *)txBuffer, n, 0xfff);
  //......................................

       va_end(args);
}

void V0fa_JustFloat(float *_data, uint8_t _num) {
  uint8_t tempData[100];
  uint32_t temp_end[4] = {0,0,0x80,0x7f};
  float temp_copy[_num];
  memcpy(&temp_copy, _data, _num * sizeof(float));
  memcpy(&tempData[_num * 4], &temp_end[0], 4);

  HAL_UART_Transmit_DMA(&UART_VOFA, (uint8_t *)tempData, (_num+1)*4);
}
