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