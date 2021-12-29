'''
easy_serial uses the PySerial library to create some useful functions
that make serial communication with python very easy. 

========
Example:
========
import easy_serial as es
BOARD = 'metro'
BAUD = 115200

# Auto open serial port for the specified board and baudrate
S = es.openSerialPort(board=BOARD,baudrate=BAUD)

# Send a string
es.writeString(S,"Hello")

# Read a string
result = es.readString(S)

# Close the serial port
es.closeSerialPort(S)
    
'''


import serial
import time
import serial.tools.list_ports

# Constants
SEND_DELAY_S = 0.1 

def getBoardInfo(board):
    '''
    getBoardInfo returns a string corresponding to the name the autoDectectBoard() function
    should look for to find what the correct COM port is
    '''
    board_index = dict()
    board_index['metro'] = 'Silicon Labs CP210x USB to UART Bridge'
    board_index['heltec-esp32'] = 'CP2102 USB to UART Bridge Controller'
    board_index['elegoo-nano'] = 'USB-SERIAL CH340'

    return board_index[board]

def autoDetectPort(board):
    '''
    autoDetectPort() finds which COM port the chosen board is connected to and returns it as a string
    '''
    boardID = getBoardInfo(board)
    comport = False
    ports = list(serial.tools.list_ports.comports())
    if len(ports) > 0:
        for p in ports:
            if boardID in p[1]:
                comport = p[0]
            else:
                raise ConnectionError('{} not detected'.format(board))
    else:
        raise ConnectionError('{} not detected'.format(board))

    return comport                

def openSerialPort(comport=None,baudrate=None,board=None):
    '''
    openSerialPort() opens the serial port for the specified board, COM port, and baudrate. The COM port 
    input is optional and can be determined with the autoDetectBoard() function 
    '''
    if not comport:
        comport = autoDetectPort(board)
    elif not baudrate:
        raise Exception('Please define a baudrate')
    elif not board:
        raise Exception('{} not defined. You may need to add it to board_index.txt if desired'.format(board))
    
    S = serial.Serial(comport,baudrate,timeout=2)
    time.sleep(2)
    if S.is_open:
        print("{} opened".format(comport))
    return S

def closeSerialPort(S):
    '''
    closeSerialPort() closes the serial port if it is open
    '''

    if not S.is_open:
        print('Serial port closed')
    elif S.is_open:
        S.close()
        if not S.is_open:
            print('Serial port closed')
        else:
            raise ConnectionError("Error closing serial port")

def readString(S):
    '''
    readString() reads a line over UART and returns it as a string
    '''
    b = S.readline()
    try:
        string = b.decode('utf-8')
        string = string.rstrip()
    except:
        string = None
    return string
    
def writeString(S,msg):
    assert(isinstance(msg,str)),'Input must be string'
    msg = msg + '\n'
    msg = msg.encode('utf-8')
    S.write(msg)
    time.sleep(SEND_DELAY_S);
    return msg
