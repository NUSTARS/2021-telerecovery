import easy_serial as es
import time
import numpy as np
import datetime

S = es.openSerialPort(baudrate=115200,board='heltec-esp32')

# arr = []
t0 = time.time()
with open('./data.txt','w+') as of:
    n = 1
    while True:
        size = S.inWaiting()
        if size:
            b = S.readline(size)
            try:
                data_str = b.decode('utf-8')
                data_str = data_str.strip()
                data_str = ''.join([data_str,',{}'.format(datetime.datetime.now().time())])
            except:
                data_str = ''
            
            of.write(data_str)
            of.write('\n')
            print(n)
            n += 1
