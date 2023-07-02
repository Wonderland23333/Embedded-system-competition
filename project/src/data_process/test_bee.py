import serial
import struct

def calculate_zigbee_fcs(data):
    fcs = 0xfe  # FCS初始值
    polynomial = 0x1  # CRC-8多项式（x^4 + x^3 + x^2 + 1）

    for byte in data:
        fcs ^= byte
        for _ in range(8):
            if fcs & 0x80:
                fcs = (fcs << 1) ^ polynomial
            else:
                fcs = fcs << 1
            fcs &= 0xFF  # 限制为1字节

    return fcs


# 打开串口
ser = serial.Serial('/dev/ttyUSB0', 38400)  # 根据实际情况修改串口号和波特率
command = b'\xfe'
NA = input("输入设备NA:")
EX = input("输入数据")
command += struct.pack('B',5+len(EX.encode())) + b'\x29\x00\x02' + bytes.fromhex(NA.replace('\\x','')) + b'\x00\x00' + EX.encode()
command += bytes([calculate_zigbee_fcs(command)])
#print(command)
ser.write(command)
ser.close()
