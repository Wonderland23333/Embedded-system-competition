// 实现对模拟量数据（AI）与计算模拟量（CN）的定周期记录（以 1 分钟为周期），
// 并实现对历史数据的年月日最大、最小值（及发生时刻）的统计分析的功能

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define NUM_AI 16  // 模拟量输入通道数量
#define NUM_CN 4   // 计算模拟量通道数量
#define MAX_HIST_RECORDS 10000 // 最大历史记录数

// 历史记录结构体
typedef struct {
    int year;
    int month;
    int day;
    double value;
} HistoryRecord;

// 模拟量输入数据结构体
typedef struct {
    double value;
    double raw_value;
    int status; // 状态: 0=正常，1=越上限，2=越下限，3=越死区
} AI;

// 计算模拟量结构体
typedef struct {
    char formula[100]; // 计算公式，例如: "AI1+AI2*2"
    double value;      // 计算结果
} CN;

// 历史记录数组
HistoryRecord hist_records[MAX_HIST_RECORDS] = {0};
int num_hist_records = 0;

// 模拟量输入数据数组
AI ai_array[NUM_AI] = {0};

// 计算模拟量数组
CN cn_array[NUM_CN] = {
  {"AI1+AI2", 0},
  {"(AI3-AI4)/2", 0},
  {"(AI5+AI6)*2", 0},
  {"(AI7*AI8)/AI9", 0}
};

// 统计最大最小值
void stat_max_min_value(double *max_value, double *min_value, int *max_index, int *min_index) {
    *max_value = hist_records[0].value;
    *min_value = hist_records[0].value;
    *max_index = 0;
    *min_index = 0;
    for (int i = 1; i < num_hist_records; i++) {
        if (hist_records[i].value > *max_value) {
            *max_value = hist_records[i].value;
            *max_index = i;
        }
        if (hist_records[i].value < *min_value) {
            *min_value = hist_records[i].value;
            *min_index = i;
        }
    }
}

// 根据索引获取历史记录日期字符串
char* get_date_str_by_index(int index) {
    static char date_str[20];
    sprintf(date_str, "%04d-%02d-%02d", hist_records[index].year, hist_records[index].month, hist_records[index].day);
    return date_str;
}

// 定时器回调函数
void timer_callback(int sig) {
    // 记录当前时间
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    // 将记录添加到历史数据中
    if (num_hist_records < MAX_HIST_RECORDS) {
        hist_records[num_hist_records].year = tm->tm_year + 1900;
        hist_records[num_hist_records].month = tm->tm_mon + 1;
        hist_records[num_hist_records].day = tm->tm_mday;
        hist_records[num_hist_records].value = cn_array[0].value; // 取第一个计算模拟量的值作为历史记录值
num_hist_records++;
}


// 更新模拟量输入数据
for (int i = 0; i < NUM_AI; i++) {
    // 模拟量输入数据随机波动
    ai_array[i].raw_value += (rand() % 200 - 100) / 10.0;
    if (ai_array[i].raw_value > 100) {
        ai_array[i].status = 1; // 越上限
        ai_array[i].raw_value = 100;
    } else if (ai_array[i].raw_value < 0) {
        ai_array[i].status = 2; // 越下限
        ai_array[i].raw_value = 0;
    } else if (ai_array[i].raw_value > 40 && ai_array[i].raw_value < 60) {
        ai_array[i].status = 3; // 越死区
    } else {
        ai_array[i].status = 0; // 正常
    }
    ai_array[i].value = ai_array[i].raw_value;
}

// 更新计算模拟量值
for (int i = 0; i < NUM_CN; i++) {
    char *formula = cn_array[i].formula;
    for (int j = 0; j < strlen(formula); j++) {
        if (formula[j] == 'A' && formula[j+1] == 'I') {
            int ai_index = formula[j+2] - '0' - 1;
            char ai_str[10];
            sprintf(ai_str, "%.2f", ai_array[ai_index].value);
            memcpy(formula+j, ai_str, strlen(ai_str));
            j += strlen(ai_str) - 1;
        }
    }
    cn_array[i].value = atof(formula);
}

}

int main() {
    // 初始化模拟量输入数据数组
    for (int i = 0; i < NUM_AI; i++) {
    ai_array[i].raw_value = 50;
    ai_array[i].value = 50;
    ai_array[i].status = 0;
    }


    // 初始化历史记录数组
    for (int i = 0; i < MAX_HIST_RECORDS; i++) {
        hist_records[i].year = 0;
        hist_records[i].month = 0;
        hist_records[i].day = 0;
        hist_records[i].value = 0;
    }

    // 安装定时器
    struct itimerval timer_val;
    timer_val.it_value.tv_sec = 1;
    timer_val.it_value.tv_usec = 0;
    timer_val.it_interval.tv_sec = 1;
    timer_val.it_interval.tv_usec = 0;
    signal(SIGALRM, timer_callback);
    setitimer(ITIMER_REAL, &timer_val, NULL);

    // 循环输出模拟量值和计算模拟量值
    while (1) {
        printf("AI: ");
        for (int i = 0; i < NUM_AI; i++) {
            printf("%.2f(%d) ", ai_array[i].value, ai_array[i].status);
        }
        printf("CN: ");
        for (int i = 0; i < NUM_CN; i++) {
            printf("%.2f ", cn_array[i].value);
        }
        printf("\n");

        // 暂停1秒钟
        sleep(1);
        // 更新模拟量输入数据
        for (int i = 0; i < NUM_AI; i++) {
            double new_value = ai_array[i].raw_value + ((double)rand() / RAND_MAX - 0.5) * 2;
            if (new_value < 0) {
                new_value = 0;
                ai_array[i].status = 2;
            } else if (new_value > 10) {
                new_value = 10;
                ai_array[i].status = 1;
            } else if (new_value > 4.5 && new_value < 5.5) {
                new_value = 5;
                ai_array[i].status = 3;
            } else {
                ai_array[i].status = 0;
            }
            ai_array[i].value = new_value;
            ai_array[i].raw_value = new_value;
        }

        // 更新计算模拟量数据
        for (int i = 0; i < NUM_CN; i++) {
            char *formula = cn_array[i].formula;
            int num_ai_used = 0;
            double sum_ai_value = 0;
            for (int j = 0; j < strlen(formula); j++) {
                char ch = formula[j];
                if (ch >= 'A' && ch <= 'Z') {
                    int ai_index = ch - 'A';
                    if (ai_index < NUM_AI) {
                        num_ai_used++;
                        sum_ai_value += ai_array[ai_index].value;
                    }
                }
            }
            if (num_ai_used > 0) {
                cn_array[i].value = sum_ai_value / num_ai_used;
            } else {
                cn_array[i].value = 0;
            }
        }
    }
}