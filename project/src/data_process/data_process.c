#include"data_process.h"

float temper() //温度获取
{
    float voltage = 0.0;
    float temperature = 0.0;
    voltage = read_adc_temper();
    temperature = (voltage - SENSOR_B) / SENSOR_A; // 根据特性曲线计算出工程值
    printf("Temperature: %f\n", temperature);
  //  data_handle(&temperature);
    return temperature;
}
float electric()//电流获取
{
    float current_ratio = 10.0;   // 电流传感器比例因子
    float current_setpoint = 5.0; // 设定电流
    float Kp = 1.0;               // PID比例系数
    float Ki = 0.01;              // PID积分系数
    float Kd = 0.0;               // PID微分系数
    float integral = 0.0;         // PID积分量
    float previous_error = 0.0;   // 上一次误差
    float digital_output;
  //  while(1) {
        // 读取电流传感器
        int sensor_reading = read_adc_electric();

        // 将传感器的读数转换为电流
        float current = convert_adc_reading_to_current(sensor_reading, current_ratio);

        // 处理电流信号并输出到DAC输出
        float pid_output = pid_controller(current_setpoint, current, Kp, Ki, Kd);
        digital_output = convert_voltage_to_digital(pid_output);
        send_digital_value_to_dac(digital_output);
  //  }
    return digital_output;
}
float read_adc_temper()
{
    int adc_value = 0;
    // 假设读取AD转换器返回采样值
    // 这里用一个随机数代替
    adc_value = rand() % (ADC_RESOLUTION + 1);
    return (float)adc_value / ADC_RESOLUTION * 5.0; // 转换为电压值
}

void data_handle(float *temperature,float *electric)
{
    float temp = *temperature;
    if(LOW_TEMPER <= temp <=HIGH_temper)
    return ;
    else
    return ;//蜂鸣器警告
}

long long get_timestamp(void)//获取时间戳函数
{
    long long tmp;
    struct timeval tv;
    gettimeofday(&tv,0);
    tmp = tv.tv_sec;
    tmp = tmp * 1000;
    tmp = tmp + (tv.tv_usec / 1000);
    return tmp;
}
void get_format_time_string(char *str_time) //获取格式化时间
{
	time_t now;
    struct tm *tm_now;
    char datetime[128];
    time(&now);
    tm_now = localtime(&now);
    strftime(datetime, 128, "%Y-%m-%d %H:%M:%S", tm_now);
 
    printf("now datetime : %s\n", datetime);
	strcpy(str_time, datetime);
}
// void insert_mysql(float *temperature,float *electric ,long long *time)
// {
//     char *sql;
//    // sscanf();
// }
float convert_adc_reading_to_voltage(int adc_reading) {
    float voltage = (adc_reading * ADC_REF_VOLTAGE) / ADC_RESOLUTION;
    return voltage;
}

float convert_adc_reading_to_current(int adc_reading, float current_ratio) {
    float voltage = convert_adc_reading_to_voltage(adc_reading);
    float current = voltage / current_ratio;
    return current;
}

int convert_voltage_to_digital(float voltage) {
    int digital_value = (int)(voltage * ADC_RESOLUTION / ADC_REF_VOLTAGE);
    return digital_value;
}

void send_digital_value_to_dac(int digital_value) {
    // 这里是DAC转换器数据传输代码
    printf("DAC输出: %d\n", digital_value);
}

int read_adc_electric(void) {
    // 这里是ADC读取数据的代码
    int adc_reading = rand() % ADC_RESOLUTION; // 模拟随机读数
    printf("ADC读取:%d\n", adc_reading);
    return adc_reading;
}

float pid_controller(float current_setpoint, float current_measured, float Kp, float Ki, float Kd) {
    static float integral = 0.0;       // PID积分量
    static float previous_error = 0.0; // 上一次误差
    // 这里是PID控制器实现代码
    float error = current_setpoint - current_measured;
    integral += error * 0.01; // 积分时间常量为0.01秒
    float derivative = (error - previous_error) / 0.01;
    float output = Kp * error + Ki * integral + Kd * derivative;
    previous_error = error;
    return output;
}