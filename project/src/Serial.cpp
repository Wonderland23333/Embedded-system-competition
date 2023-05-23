#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

// 串口配置
const char* SERIAL_PORT = "/dev/ttyUSB0";
const int SERIAL_BAUDRATE = B9600;

// 串口通信类
class SerialCommunication {
private:
    int fd;

public:
    SerialCommunication() : fd(-1) {}

    bool openSerialPort(char* serial_port, int serial_baudrate) {
        fd = open(serial_port, O_RDWR | O_NOCTTY | O_NDELAY);
        if (fd == -1) {
            std::cerr << "Failed to open serial port." << std::endl;
            return false;
        }

        struct termios options;
        if(tcgetattr(fd, &options) == -1)return false;
        if(cfsetispeed(&options, serial_baudrate) == -1)return false;//设置输入波特率
        if(cfsetospeed(&options, serial_baudrate) == -1)return false;//设置输出波特率
        options.c_cflag |= (CLOCAL | CREAD);
        if(tcsetattr(fd, TCSANOW, &options) == -1)return false;
        

        return true;
    }

    void closeSerialPort() {
        if (fd != -1) {
            close(fd);
            fd = -1;
        }
    }

    bool sendSerialData(const std::string& data) {
        if (fd == -1) {
            std::cerr << "Serial port is not open." << std::endl;
            return false;
        }

        write(fd, data.c_str(), data.length());
        return true;
    }
};

int main() {
    SerialCommunication serialComm;

    // 用户设置通信参数示例
    // TODO: 根据用户需求设置波特率、数据位、停止位等
    printf("请输入波特率：（形式：B波特率）\n");
    int baud_rate;
    std::cin>>baud_rate;
    printf("请输入串口设备文件路径：\n");
    char* serial_path;
    std::cin>>serial_path;

    // 打开串口
    if (!serialComm.openSerialPort(serial_path,baud_rate)) {
        std::cerr << "Failed to open serial port. Exiting." << std::endl;
        return 1;
    }

    std::string message = "Hello, World!";

    // 发送数据示例
    if (!serialComm.sendSerialData(message)) {
        std::cerr << "Failed to send data through serial port." << std::endl;
    }

    // 关闭串口
    serialComm.closeSerialPort();

    return 0;
}
