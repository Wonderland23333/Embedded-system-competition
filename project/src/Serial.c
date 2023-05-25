#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define BAUD_B0 B0
#define BAUD_B50 B50
#define BAUD_B75 B75
#define BAUD_B110 B110
#define BAUD_B134 B134
#define BAUD_B150 B150
#define BAUD_B200 B200
#define BAUD_B300 B300
#define BAUD_B600 B600
#define BAUD_B1200 B1200
#define BAUD_B1800 B1800
#define BAUD_B2400 B2400
#define BAUD_B4800 B4800
#define BAUD_B9600 B9600
#define BAUD_B19200 B19200
#define BAUD_B38400 B38400

int convert_baud(const char* a) {
    if (strcmp(a, "B0") == 0) return BAUD_B0;
    else if (strcmp(a, "B50") == 0) return BAUD_B50;
    else if (strcmp(a, "B75") == 0) return BAUD_B75;
    else if (strcmp(a, "B110") == 0) return BAUD_B110;
    else if (strcmp(a, "B134") == 0) return BAUD_B134;
    else if (strcmp(a, "B150") == 0) return BAUD_B150;
    else if (strcmp(a, "B200") == 0) return BAUD_B200;
    else if (strcmp(a, "B300") == 0) return BAUD_B300;
    else if (strcmp(a, "B600") == 0) return BAUD_B600;
    else if (strcmp(a, "B1200") == 0) return BAUD_B1200;
    else if (strcmp(a, "B1800") == 0) return BAUD_B1800;
    else if (strcmp(a, "B2400") == 0) return BAUD_B2400;
    else if (strcmp(a, "B4800") == 0) return BAUD_B4800;
    else if (strcmp(a, "B9600") == 0) return BAUD_B9600;
    else if (strcmp(a, "B19200") == 0) return BAUD_B19200;
    else if (strcmp(a, "B38400") == 0) return BAUD_B38400;

    return -1;
}

// 串口配置
const char* SERIAL_PORT = "/dev/ttyUSB0";

int openSerialPort(const char* serial_port, int serial_baudrate) {
    int fd = open(serial_port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        printf("Failed to open serial port.\n");
        return -1;
    }

    struct termios options;
    if (tcgetattr(fd, &options) == -1) return -1;
    if (cfsetispeed(&options, serial_baudrate) == -1) return -1;
    if (cfsetospeed(&options, serial_baudrate) == -1) return -1;
    options.c_cflag |= (CLOCAL | CREAD);
    if (tcsetattr(fd, TCSANOW, &options) == -1) return -1;

    return fd;
}

void closeSerialPort(int fd) {
    if (fd != -1) {
        close(fd);
    }
}

int sendSerialData(int fd, const char* data) {
    if (fd == -1) {
        printf("Serial port is not open.\n");
        return -1;
    }

    int length = strlen(data);
    if (write(fd, data, length) != length) {
        printf("Failed to send data through serial port.\n");
        return -1;
    }

    return 0;
}

int main() {
    int fd;

    // 用户设置通信参数示例
    // TODO: 根据用户需求设置波特率、数据位、停止位等
    char baud_rate_c[10];
    int baud_rate;

G:
    printf("请输入波特率：(B+波特率)\n");
    scanf("%s", baud_rate_c);

    baud_rate = convert_baud(baud_rate_c);
    if (baud_rate == -1) {
        printf("Invalid baud rate!\n");
        goto G;
    }

    printf("请输入串口设备文件路径：\n");
    char serial_path[20];
    scanf("%s", serial_path);

    // 打开串口
    fd = openSerialPort(serial_path, baud_rate);
    if (fd == -1) {
        printf("Failed to open serial port. Exiting.\n");
        return 1;
    }

    const char* message = "Hello, World!";

    // 发送数据示例
    if (sendSerialData(fd, message) == -1) {
        printf("Failed to send data through serial port.\n");
    }

    // 关闭串口
    closeSerialPort(fd);

    return 0;
}
