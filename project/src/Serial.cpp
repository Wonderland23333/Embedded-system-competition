#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
using namespace std;

int convert_baud(string a){
    if(a == "B0")return 0;
    else if(a == "B50")return 1;
    else if (a == "B75") return 2;
    else if (a == "B110") return 3;
    else if (a == "B134") return 4;
    else if (a == "B150") return 5;
    else if (a == "B200") return 6;
    else if (a == "B300") return 7;
    else if (a == "B600") return 8;
    else if (a == "B1200") return 9;
    else if (a == "B1800") return 10;
    else if (a == "B2400") return 11;
    else if (a == "B4800") return 12;
    else if (a == "B9600") return 13;
    else if (a == "B19200") return 14;
    else if (a == "B38400") return 15;

    return -1;
}
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
            cerr << "Failed to open serial port." << endl;
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

    bool sendSerialData(const string& data) {
        if (fd == -1) {
            cerr << "Serial port is not open." << endl;
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
    G:printf("请输入波特率：(B+波特率)\n");
    string baud_rate_c;
    cin>>baud_rate_c;

    int baud_rate = convert_baud(baud_rate_c);
    if(baud_rate == -1){
        cout<<"worry baud rate!"<<endl;
        goto G;
    }

    printf("请输入串口设备文件路径：\n");
    char* serial_path;
    cin>>serial_path;
    
    // 打开串口
    if (!serialComm.openSerialPort(serial_path,baud_rate)) {
        cerr << "Failed to open serial port. Exiting." << endl;
        return 1;
    }

    string message = "Hello, World!";

    // 发送数据示例
    if (!serialComm.sendSerialData(message)) {
        cerr << "Failed to send data through serial port." << endl;
    }

    // 关闭串口
    serialComm.closeSerialPort();

    return 0;
}
