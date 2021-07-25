import json

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
