import serial

import re

import sqlite3


ser = serial.Serial('/dev/ttyUSB0', 38400, timeout=2000)

data_buffer = ''  # 数据缓冲区
data_dir = {}


HIGH_TEMPER =85.0

LOW_TEMPER =-40.0



HIGH_HUM = 10.0

LOW_HUM = 90.0



HIGH_LIGHT = 1000.0

LOW_LIGHT = 1200.0



def parse_data(data):

    global data_buffer



    # 将新获取的数据添加到缓冲区

    data_buffer += data



    # 使用正则表达式提取花括号内的内容

    pattern = r"\{(.*?)\}"

    matches = re.findall(pattern, data_buffer)

    

    # 提取完成后更新缓冲区，去除已经处理过的数据

    last_match_end = 0

    for match in matches:

        match_start = data_buffer.index(match, last_match_end)

        match_end = match_start + len(match)

        last_match_end = match_end



    data_buffer = data_buffer[last_match_end:]

    

    return matches



def data_handle(process_data):
    conn = sqlite3.connect("/home/loongson/Desktop/Embedded-system-competition-lihao/project/sensor.db")
    data_list=process_data[0]

    datas = re.findall(r'\d+(?:\.\d+)?',data_list)

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

    tem_handle(temperature)

    hum_handle(humility)

    light_handle(lighten)
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


def tem_handle(temperature):

    if LOW_TEMPER>float(temperature):

        print('temper low')

    elif float(temperature)>HIGH_TEMPER:

        print('temper high')

    else:

        return None



def hum_handle(humility):

    if float(humility)<LOW_HUM:

        print('hum low')

    elif float(humility)>HIGH_HUM:

        print('hum high')

    else:

        return None



def light_handle(lighten):

    if float(lighten)<LOW_LIGHT:

        print("lighten low")

    elif float(lighten)>HIGH_LIGHT:

        print("lighten high")

    else:

        return None    



conn = sqlite3.connect("/home/loongson/Desktop/Embedded-system-competition-lihao/project/sensor.db")
base_sql ="SELECT name FROM sqlite_master WHERE type='table' AND name='SOLAR'"
if conn.execute(base_sql).fetchone() == None:
    conn.execute('''CREATE TABLE SOLAR
        (temperature float     NOT NULL,
        humility    float           NOT NULL,
        lighten     float     NOT NULL);''')
conn.close()
while True:

    data = ser.read_all().decode('utf-8', errors='ignore')
    if data:  # 只处理非空数据

        process_data = parse_data(data)

        if process_data:  # 判断是否有匹配到的内容

            print(process_data)

            data_handle(process_data)

            



