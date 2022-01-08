import matplotlib.pyplot as plt 
plt.style.use('ggplot')

temp = []
tof = []
P = []
time = []
# tof,T,P,Alt,ax,ay,az,gx,gy,gz,mx,my,mz,timestamp
with open('data.txt','r') as f:
    for row in f:
        row = row.strip().split(",")
        temp.append(float(row[1]))
    
print(time)
print(temp)
fig,ax = plt.subplots()
ax.plot(temp,'-ob')
plt.show()