#ifndef _data_process_H
#define _data_process_H
#define buffersize 1024
#define BUF_SIZE 1024
#define CRTSCTS  0x80000000
#define PORT "/dev/ttyS0"
#define BAUDRATE B9600
#define HIGH_temper 35.0f
#define LOW_TEMPER  15.0f
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sqlite3.h>
//#define ADC_RESOLUTION 1024

// 定义温度传感器特性曲线参数
#define SENSOR_A 0.00125 // 斜率
#define SENSOR_B 2.5     // 截距
#define AI_TEMP 24.5 // AI温度量
#define DI_SWITCH 1 // DI开关量，1为打开，0为关闭
#define ADC_REF_VOLTAGE 3.3    // ADC参考电压
#define ADC_RESOLUTION 4095    // ADC分辨率


void data_handle(float *temperature,float *electric);//数据处理
void insert_mysql(float *temperature,long long *time);//录入数据库
long long get_timestamp(void);//获取时间戳函数
void get_format_time_string(char *str_time);//获取格式化时间
float temper();//获取温度
float electric();//电流获取
float read_adc_temper();//AD温度采集数据
float convert_adc_reading_to_voltage(int adc_reading);
float convert_adc_reading_to_current(int adc_reading, float current_ratio);
int convert_voltage_to_digital(float voltage);
void send_digital_value_to_dac(int digital_value) ;
int read_adc_electric(void) ;
float pid_controller(float current_setpoint, float current_measured, float Kp, float Ki, float Kd);
#endif
