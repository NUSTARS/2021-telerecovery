'''
I used this code for debugging the format of the data sent over LoRa. I'm including it
since it might be useful for debugging/testing stuff for the groundstation stuff.

This script will read data over serial from the reciever LoRa board. It then writes that 
data to a file called data.txt in the directory the script is run. 

'''


import easy_serial as es
import time
import numpy as np
import datetime

S = es.openSerialPort(baudrate=115200,board='heltec-esp32')

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
