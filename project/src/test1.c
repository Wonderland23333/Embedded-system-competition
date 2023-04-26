#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 1024


// TODO: 添加温湿度传感器初始化代码
// cellect inf
int cl(){
    int fd;
    char buf[BUF_SIZE];

    // 打开串口设备文件
    fd = open("/dev/ttyS0", O_RDWR);
    if (fd < 0) {
        perror("Failed to open serial port");
        return 1;
    }

    // 配置串口参数
    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(fd, &tty) != 0) {
        perror("Failed to get serial port attributes");
        close(fd);
        return 1;
    }

    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_oflag &= ~OPOST;
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 5;
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("Failed to set serial port attributes");
        close(fd);
        return 1;
    }

    // 从串口读取温度和湿度数据
    printf("Reading temperature and humidity data from serial port...\n");
    while (1) {
        ssize_t len = read(fd, buf, BUF_SIZE);
        if (len > 0) {
            buf[len] = '\0';
            printf("Received data: %s", buf);
        } else if (len < 0) {
            perror("Failed to read from serial port");
            close(fd);
            return 1;
        }
    }

    close(fd);
    return 0;
}
#define PORT "/dev/ttyS0"
#define BAUDRATE B9600

int main()
{
    int fd;
    struct termios oldtio, newtio;
    char buf[256];

    fd = open(PORT, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror(PORT);
        exit(1);
    }

    tcgetattr(fd, &oldtio);
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR | ICRNL;
    newtio.c_oflag = 0;
    newtio.c_lflag = ICANON;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    while (1) {
        // TODO: 采集温度和湿度数据
        cl();
        float temperature = 25.0;
        float humidity = 60.0;

        // 发送数据到串口
        sprintf(buf, "Temperature: %.1fC, Humidity: %.1f%%\n", temperature, humidity);
        write(fd, buf, strlen(buf));
        usleep(1000000);
    }

    tcsetattr(fd, TCSANOW, &oldtio);
    close(fd);

    return 0;
}

