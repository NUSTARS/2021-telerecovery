import json

'''
# Read serial port with something like this 
BAUD = 115200
COM_PORT = 'COM4'
with serial.Serial(port=COM_PORT, baudrate=BAUD, timeout=1) as ser:
  x = ser.read()          # read one byte
  s = ser.read(10)        # read up to ten bytes (timeout)
  line = ser.readline()   # read a '\n' terminated line
'''
# a Python object (dict):
x = {
  "Time": "00:00:01",
  "Main": 1,
  "Drogue": 0,
}

# convert into JSON and save
with open('./data.json','a+') as out:
  json.dump(x,out)
y = json.dumps(x)

print(y)
