import serial
import time
import struct


import re



import sqlite3





ser = serial.Serial('/dev/ttyUSB0', 38400, timeout=2000)



data_buffer = ''  # 数据缓冲区

data_dir = {}

data_b = ""
length = 0

EX=""#输入数据
NA_B=""
NA_A=""


open_bee="{OD1=8,D1=?}"
close_bee="{CD1=8,D1=?}"

HIGH_TEMPER =85.0



LOW_TEMPER =-40.0







HIGH_HUM = 10.0



LOW_HUM = 90.0







HIGH_LIGHT = 1000.0



LOW_LIGHT = 1200.0




def parse_data(hex_data):
    global data_b,length,NA_A,NA_B
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
                
                #print(decoded_content)
                if "," not in decoded_content:
                    print("当前设备NA_B：" + result[16:24])
                    NA_B=result[16:24]
                else:
                    print("当前设备NA_A：" + result[16:24])
                    NA_A = result[16:24]
                return decoded_content
            except:
                return None
        data_b += hex_data
    else:
        data_b = hex_data
        return None



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


def send_command(NA_B,EX):
    command = b'\xfe'
    command += struct.pack('B',5+len(EX.encode())) + b'\x29\x00\x02' + bytes.fromhex(NA_B.replace('\\x','')) + b'\x00\x00' + EX.encode()
    command += bytes([calculate_zigbee_fcs(command)])
    print(command)
    ser.write(command)



def data_handle(process_data,NA_B):

    conn = sqlite3.connect("sensor.db")
    if "," not in process_data:
        return None
    

    datas = re.findall(r'\d+(?:\.\d+)?',process_data)



    temperature = datas[1]



    humility =    datas[3]



    lighten =     datas[5]



    #air_quality = datas[7]



    #air_pressure= datas[9]



    print(temperature)



    print(humility)



    print(lighten)



    #print(air_quality)



    #print(air_pressure)



    tem_handle(temperature,NA_B)



    hum_handle(humility,NA_B)



    light_handle(lighten,NA_B)

    temperature = float(temperature)

    humility=float(humility)

    lighten=float(lighten)

    data_dir['temperature']=temperature

    data_dir['humility'] = humility

    data_dir['lighten'] = lighten

    sql = "insert into SOLAR(temperature,humility,lighten)values(%f,'%f',%f)" % (data_dir['temperature'],data_dir['humility'],data_dir['lighten'])

    conn.execute(sql)

    conn.commit()

    conn.close()





def tem_handle(temperature,NA_B):



    if LOW_TEMPER>float(temperature):
        EX=open_bee
        send_command(NA_B,EX)
        print('temper low')



    elif float(temperature)>HIGH_TEMPER:
        EX=open_bee
        send_command(NA_B,EX)
        print('temper high')
    else:



        return None







def hum_handle(humility,NA_B):



    if float(humility)<LOW_HUM:
        EX=open_bee
        send_command(NA_B,EX)
        print('hum low')



    elif float(humility)>HIGH_HUM:



        print('hum high')



    else:

        return None







def light_handle(lighten,NA_B):



    if float(lighten)<LOW_LIGHT:



        print("lighten low")



    elif float(lighten)>HIGH_LIGHT:



        print("lighten high")



    else:



        return None    







conn = sqlite3.connect("sensor.db")

base_sql ="SELECT name FROM sqlite_master WHERE type='table' AND name='SOLAR'"

if conn.execute(base_sql).fetchone() == None:

    conn.execute('''CREATE TABLE SOLAR

        (temperature float     NOT NULL,

        humility    float           NOT NULL,

        lighten     float     NOT NULL);''')

conn.close()
while True:

    data = ser.read_all()

    if data:  # 只处理非空数据
        hex_data = ''.join('\\x{:02x}'.format(byte) for byte in data)
        #print(type(hex_data))
        result = parse_data(hex_data)
        if result:
            print(result+": 数据长度<"+str(len(result))+">字节")
            data_handle(result,NA_B)


            







