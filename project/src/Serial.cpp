#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
using namespace std;
int convert_dataBits(const string& a) {
    if (a == "5") return CS5;
    else if (a == "6") return CS6;
    else if (a == "7") return CS7;
    else if (a == "8") return CS8;

    return -1;
}
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
const int SERIAL_DATABITS = CS8;


// 串口通信类
class SerialCommunication {
private:
    int fd;

public:
    SerialCommunication() : fd(-1) {}

    bool openSerialPort() {
        fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
        if (fd == -1) {
            cerr << "Failed to open serial port." << endl;
            return false;
        }

        struct termios options;
        if(tcgetattr(fd, &options) == -1)return false;
        if(cfsetispeed(&options, SERIAL_BAUDRATE) == -1)return false;//设置输入波特率
        if(cfsetospeed(&options, SERIAL_BAUDRATE) == -1)return false;//设置输出波特率
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
    //设置串口位置
    bool configureSerialPort(char* serial_port){
        fd = open(serial_port, O_RDWR | O_NOCTTY | O_NDELAY);
        if (fd == -1) {
            cerr << "Failed to open serial port." << endl;
            return false;
        }
        return true;
    }
    //设置串口波特率
    bool configureBaudrate(int serial_baudrate){
        if (fd == -1) {
            cerr << "Failed to open serial port." << endl;
            return false;
        }

        struct termios options;
        if(tcgetattr(fd, &options) == -1)return false;

        if(cfsetispeed(&options, serial_baudrate) == -1)return false;//设置输入波特率
        if(cfsetospeed(&options, serial_baudrate) == -1)return false;//设置输出波特率

        if(tcsetattr(fd, TCSANOW, &options) == -1)return false;

        return true;
    }
    //设置串口数据位
    bool configureDataBits(int dataBits) {
        if (fd == -1) {
            cerr << "Serial port is not open." << endl;
            return false;
        }

        struct termios options;
        if (tcgetattr(fd, &options) == -1) return false;

        // 清除现有的数据位设置
        options.c_cflag &= ~CSIZE;

        // 设置新的数据位
        options.c_cflag |= dataBits;

        if (tcsetattr(fd, TCSANOW, &options) == -1) return false;

        return true;
    }

    //设置串口停止位
    bool configureCSTOPB(int bit){
        if (fd == -1) {
            cerr << "Serial port is not open." << endl;
            return false;
        }
        struct termios options;
        if (tcgetattr(fd, &options) == -1) return false;
        if(bit == 1){
            options.c_cflag &= ~CSTOPB;  // 1位停止位
        }else if(bit == 2){
            options.c_cflag |= CSTOPB;   // 2位停止位
        }else{
            return false;
        }
        return true;
    }

    //设置串口校验位
    bool configureParity(int sets){
        if (fd == -1) {
            cerr << "Serial port is not open." << endl;
            return false;
        }
        struct termios options;
        if (tcgetattr(fd, &options) == -1) return false;

        switch (sets)
        {
        case 0/* constant-expression */:
            options.c_cflag |= PARENB;/* 启用校验位 */
            break;

        case 1:
            options.c_cflag &= ~PARODD;/* 偶校验 */
            break;
        
        case 2:
            options.c_cflag |= PARODD;/* 奇校验 */
            break;

        case 3:
            options.c_cflag &= ~PARENB;/* 禁用校验位 */
            break;

        default:
            return false;
        }
        return true;
    }

    //设置流控制
    bool configureFlow(int sets){
        if (fd == -1) {
            cerr << "Serial port is not open." << endl;
            return false;
        }
        struct termios options;
        if (tcgetattr(fd, &options) == -1) return false;

        switch (sets)
        {
        case 0/* constant-expression */:
            options.c_cflag |= CRTSCTS;/* 启用硬件流控制 */
            break;

        case 1:
            options.c_iflag |= (IXON | IXOFF);/* 启用软件流控制 */
            break;

        default:
            return false;
        }
        return true;
    }

    //超时设置
    bool configureVtimes(int times){
        times *= 10;
        if (fd == -1) {
            cerr << "Serial port is not open." << endl;
            return false;
        }
        struct termios options;
        if (tcgetattr(fd, &options) == -1) return false;

        options.c_cc[VTIME] = times;
        return true;
    }

    //设置最小读取字符
    bool configureVmin(int size){
        if (fd == -1) {
            cerr << "Serial port is not open." << endl;
            return false;
        }
        struct termios options;

        options.c_cc[VMIN] = size;
        return true;
    }

    //发送数据
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
    // 打开串口
    if (!serialComm.openSerialPort()) {
        cerr << "Failed to default serial port." << endl;
        return 1;
    }

    // 用户设置通信参数示例
    // 根据用户需求设置串口位置、波特率、数据位、停止位
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
    
    printf("请输入数据位（5、6、7、8）：\n");
    string dataBits;
    cin >> dataBits;
    //转换数据位
    int dataBitsValue = convert_dataBits(dataBits);
    if (dataBitsValue == -1) {
        cout << "无效的数据位设置！" << endl;
        goto G;
    }
    
    printf("请输入停止位（1、2）：\n");
    int cstopb;
    cin>>cstopb;
    if(cstopb != 1 || cstopb != 2){
        cout<< "无效的停止位设置！"<<endl;
        goto G;
    }

    

    printf("是否启用串口校验位？（y ? others）\n");
    string judge;
    cin>>judge;
    if(judge == "y"){
        serialComm.configureParity(0);
        printf("输入校验方式：\n偶校验:1\n奇校验:2\n无校验:3\n");
        int sets;
        cin>>sets;
        if(!serialComm.configureParity(sets)){
            cerr<<"Failed to configure Parity"<<endl;
        }
    }

    printf("输入超时时间：(整数)单位s\n");
    int times;
    cin>>times;
    if(!serialComm.configureVtimes(times)){
        cerr<<"Failed to configure Vtimes"<<endl;
    }

    printf("输入最小读取字符：(整数)\n");
    int bits;
    cin>>bits;
    if(!serialComm.configureVmin(times)){
        cerr<<"Failed to configure Vmin"<<endl;
    }


    
    //设置串口设备文件
    if(!serialComm.configureSerialPort(serial_path)){
        cerr << "Failed to open serial port." << endl;
        return 1;
    }
    //设置串口波特率
    if(serialComm.configureBaudrate(baud_rate)){
        cerr << "Failed to configure serial baudrate." << endl;
        return 1;
    }
    //设置数据位
    if (!serialComm.configureDataBits(dataBitsValue)) {
        cerr << "Failed to configure data bits." << endl;
    }
    //设置停止位
    if(!serialComm.configureCSTOPB(cstopb)){
        cerr << "Failed to configure stop bits."<<endl;
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
