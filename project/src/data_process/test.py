import serial
import time
import struct
import re
# 打开串口
ser = serial.Serial('/dev/ttyUSB0', 38400)  # 根据实际情况修改串口号和波特率

# 需要发送的原始数据（字节数据）
raw_data = b'\xfe\x1f\x29\x00\x02\x00\x00\x00\x00\x7b\x54\x59\x50\x45\x3d\x3f\x2c\x50\x41\x4e\x49\x44\x3d\x3f\x2c\x43\x48\x41\x4e\x4e\x45\x4c\x3d\x3f\x7d\x39'
#raw_data = b'\xFE'+b'\x11\x29\x00\x02\xA8\x53\x00\x00\x7B\x4F\x44\x31\x3D\x38\x2C\x44\x31\x3D\x3F\x7D\xA3'
# 发送数据
#close_d = b'\xFE\x11\x29\x00\x02\xA8\x53\x00\x00\x7B\x43\x44\x31\x3D\x38\x2C\x44\x31\x3D\x3F\x7D\xAF'
ser.write(raw_data)
data_b = ""
length = 0
def parse_data(hex_data):
    global data_b,length
    if data_b:
        #data_b = repr(data_b)[2:-1]
        #print(data_b+": "+str(len(data_b)))
        if len(data_b) == 4:
            #print(hex_data[2:4])
            #print(bytes.fromhex(hex_data[2:4]))
            length = int.from_bytes(bytes.fromhex(hex_data[2:4]),byteorder='little')
            #print("zichang: "+str(length))
        if len(data_b)+len(hex_data) == (length+5)*4:
            result = data_b+hex_data
            data_b = ''
            length = 0
            try:
                pattern = r'\\x7b(.*?)\\x7d'
                data_c = {}
                content = re.search(pattern, result).group(1)
                content = content.replace('\\x', '')
                #print(content)
                decoded_content = bytes.fromhex(content).decode()
                print("当前设备NA：" + result[16:24])
                #print(decoded_content)
                return decoded_content
            except:
                return None
        data_b += hex_data
    else:
        data_b = hex_data
        return None

# 接收数据
while True:
    data = ser.read_all()

    if data:  # 只处理非空数据

        hex_data = ''.join('\\x{:02x}'.format(byte) for byte in data)
        #print(type(hex_data))
        result = parse_data(hex_data)
        if result:
            print(result+": 数据长度<"+str(len(result))+">字节")
        #data = ser.readall()  # 读取数据并解码
        #print(data)
time.sleep(2)
#ser.write(close_d)

# 关闭串口
ser.close()
