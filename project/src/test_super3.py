import serial

import time

import struct

import re

import sqlite3

import threading
from datetime import datetime

# 全局变量

ser = serial.Serial('/dev/ttyUSB0', 38400, timeout=2000)

data_buffer = ''  # 数据缓冲区

data_dir = {}

data_b = ""

length = 0

EX=""#输入数据

NA_B=""

NA_A=""
tem_sign=0 
hum_sign=0
light_sign=0

lock = threading.Lock()
#create table warning_list(humility float not NUll,lighten float not null,date date not null,temperature float not null,warning string not null);

open_bee_CGB="{OD1=9,D1=?}"

close_bee_CGB="{CD1=9,D1=?}"

tem_warn="temperature warn"
hum_warn="humility warn"
light_warn="lighten warn"


HIGH_TEMPER =85.0

LOW_TEMPER =-40.0

HIGH_HUM = 90.0

LOW_HUM = 10.0

HIGH_LIGHT = 100.0

LOW_LIGHT = 0





def parse_data(hex_data):

    global data_b,length,NA_A,NA_B



    if data_b:

       #data_b = repr(data_b)[2:-1]

        #print(data_b+": "+str(len(data_b)))

        #print(data_b[:4])

        if len(data_b) > 4 and length == 0 and data_b[:4] == '\\xfe':

           #print(hex_data[2:4])

           #print(bytes.fromhex(data_b[6:8]))

           length = int.from_bytes(bytes.fromhex(data_b[6:8]),byteorder='little')

           #print("zichang: "+str(length))

        if len(data_b)+len(hex_data) == (length+5)*4 and length != 0:

           if length == 1 and hex_data[-4:] == '\\x68':
               #print(data_b+"无效数据"+str(length)+"hex:"+hex_data[-4:])
               data_b = ''
               length=0
               return None
           result = data_b+hex_data

           data_b = ''

           length = 0

           try:

               pattern = r'\\x7b(.*?)\\x7d'

            #    data_c = {}

               content = re.search(pattern, result).group(1)

               content = content.replace('\\x', '')

               #print(content)

               decoded_content = bytes.fromhex(content).decode()

               

               #print(decoded_content)

               if "A0" not in decoded_content:

                   #print("设备NA_B：" + result[16:24])

                   NA_B=result[16:24]

               else:

                   #print("当前设备NA_A：" + result[16:24])

                   NA_A = result[16:24]

               return decoded_content

           except Exception as e:
               print("==出错"+str(e))
               return None



           



        if len(data_b) + len(hex_data) > (length+5)*4 and length != 0:
            if length == 1:
                result = data_b+hex_data
                data_b = result[(length+5)*4:]
                length = 0
                return None
            result = data_b+hex_data
            # print("result: "+result+": "+str(len(result)))
            data_b = result[(length+5)*4:]
            # print(length)
            # print("data: "+data_b+": "+str(len(data_b)))
            try:
                pattern = r'\\x7b(.*?)\\x7d'
            #    data_c = {}
                content = re.search(pattern, result[:(length+5)*4]).group(1)
                content = content.replace('\\x', '')
                #print(content)
                decoded_content = bytes.fromhex(content).decode()
                
                # print(decoded_content)
                if "A0" not in decoded_content:
                    #print("当前设备NA_B：" + result[16:24])
                    NA_B=result[16:24]
                else:
                    #print("当前设备NA_A：" + result[16:24])
                    NA_A = result[16:24]
                length = 0
                return decoded_content
            except Exception as e:
                print(">出错"+str(e))
                return None
        data_b += hex_data
        #print(data_b)
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

   ser.write(command)



def data_handle(process_data,NA_A,NA_B): 
   now = datetime.now()
   date=now.strftime("%Y-%m-%d %H:%M:%S")
   global tem_sign,hum_sign,light_sign

   if "A0" not in process_data:
       print(f"设备NA_B:{NA_B}")
       return None
   print(f"设备NA_A:{NA_A}")
   conn = sqlite3.connect("sensor.db")

   datas = re.findall(r'\d+(?:\.\d+)?',process_data)

   temperature = datas[1]

   humility =    datas[3]

   lighten =     datas[5]

   #air_quality = datas[7]

   #air_pressure= datas[9]

   print(f"温度：{temperature}")

   print(f"湿度：{humility}")

   print(f"光强：{lighten}")

   #print(air_quality)

   #print(air_pressure)
   temperature = float(temperature)

   humility=float(humility)

   lighten=float(lighten)
   tem_sign =tem_handle(temperature,NA_B,tem_sign)

   hum_sign = hum_handle(humility,NA_B,hum_sign)

   light_sign=light_handle(lighten,NA_B,light_sign)

   data_dir['temperature']=temperature

   data_dir['humility'] = humility

   data_dir['lighten'] = lighten
   lock.acquire()
   try:
    conn = sqlite3.connect('sensor.db')
    if tem_sign == 1:
        sql_warn = "insert into warning_list(data,date,warning)values('%f','%s','%s')" % (data_dir['temperature'],date,tem_warn)
        sql = "insert into solar(humility,lighten,date,temperature)values('%f','%f','%s','%f')" % (data_dir['humility'],data_dir['lighten'],date,data_dir['temperature'])
        conn.execute(sql)
        conn.execute(sql_warn)
        conn.commit()
    if hum_sign ==1:
        sql_warn = "insert into warning_list(data,date,warning)values('%f','%s','%s')" % (data_dir['humility'],date,hum_warn)
        sql = "insert into solar(humility,lighten,date,temperature)values('%f','%f','%s','%f')" % (data_dir['humility'],data_dir['lighten'],date,data_dir['temperature'])
        conn.execute(sql)
        conn.execute(sql_warn)
        conn.commit()
    if light_sign == 1:
        sql_warn = "insert into warning_list(data,date,warning)values('%f','%s','%s')" % (data_dir['lighten'],date,light_warn)
        sql = "insert into solar(humility,lighten,date,temperature)values('%f','%f','%s','%f')" % (data_dir['humility'],data_dir['lighten'],date,data_dir['temperature'])
        conn.execute(sql)
        conn.execute(sql_warn)
        conn.commit()
    else:
        sql = "insert into solar(humility,lighten,date,temperature)values('%f','%f','%s','%f')" % (data_dir['humility'],data_dir['lighten'],date,data_dir['temperature'])
        conn.execute(sql)
        conn.commit()
   except Exception as e:
       print("数据库操作出错:", str(e))
       conn.close()
   finally:
       lock.release()
   

def tem_handle(temperature,NA_B,tem_sign):

   if LOW_TEMPER>temperature:
       if NA_B == "":
        return None
       send_command(NA_B,open_bee_CGB)
       print('temper low')
       return 1

   elif temperature>HIGH_TEMPER:
       if NA_B == "":
        return None
       send_command(NA_B,open_bee_CGB)
       print('temper high')
       return 1

   else:
       print("temperature normal")
       if tem_sign ==1:
          send_command(NA_B,close_bee_CGB)
       return 0

   



def hum_handle(humility,NA_B,hum_sign):

   if humility<LOW_HUM:
       if NA_B == "":
        return None
       send_command(NA_B,open_bee_CGB)
       print("hum low")
       return 1

   elif humility>HIGH_HUM:
        if NA_B == "":
           return None
        send_command(NA_B,open_bee_CGB)
        print("hum high")
        return 1

   else:
        print("humidity normal")
        if hum_sign == 1:
           send_command(NA_B,close_bee_CGB)
           return 0

   



def light_handle(lighten,NA_B,light_sign):

   if lighten<LOW_LIGHT:
       if NA_B == "":
        return None
       print("lighten low")
       send_command(NA_B,open_bee_CGB)
       return 1

   elif lighten>HIGH_LIGHT:
    if NA_B == "":
        return None      
    send_command(NA_B,open_bee_CGB)
    print("lighten high")
    return 1

   else:
        print("lighten normal")
        if light_sign == 1:
            print("close CGB and bee")
            send_command(NA_B,close_bee_CGB)
            return 0





conn = sqlite3.connect("sensor.db")

base_sql ="SELECT name FROM sqlite_master WHERE type='table' AND name='solar'"
warning_sql="SELECT name FROM sqlite_master WHERE type='table' AND name='warning_list'"
if conn.execute(base_sql).fetchone() == None :

    conn.execute('''CREATE TABLE SOLAR

        (temperature float     NOT NULL,

        humility    float           NOT NULL,

        lighten     float     NOT NULL);''')
if conn.execute(warning_sql).fetchone == None :
    
    conn.execute('''CREATE table warning_list
                 (humility float not NUll,
                 lighten float not null,
                 date date not null,temperature float not null,
                 warning string not null);''')
conn.close()





def main():

    while True:

        data = ser.read_all()
        if data:  # 只处理非空数据
            #print(data)
            hex_data = ''.join('\\x{:02x}'.format(byte) for byte in data)
            
            result = parse_data(hex_data)
            if result:
                #print(result+": 数据长度<"+str(len(result))+">字节")
                # print(f"设备A:{NA_A}")
                # print(f"设备B:{NA_B}")
                data_handle(result,NA_A,NA_B)
                data=b""


if __name__ == '__main__':

    main()

           

