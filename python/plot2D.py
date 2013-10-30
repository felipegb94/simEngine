import matplotlib.pyplot as plt
import string
import json

dataFilePath = "plots/data.json"
dataFile = open(dataFilePath, "r")
data = json.load(dataFile)
dataFile.close()

plot1 = data["Plot1"]
plot2 = data["Plot2"]
x1Data = plot1[0]
x2Data = plot2[0]

y1Data = plot1[1]
y2Data = plot2[1]
plot1Name = "plot1.png"
plt.plot(x1Data, y1Data)
plt.xlabel("Time")
plt.ylabel("Acceleration")
plt.figure(1)
plt.savefig("plots/"+plot1Name)
plt.clf()
plot2Name = "plot2.png"
plt.plot(x2Data,y2Data)
plt.xlabel("x")
plt.ylabel("y")
plt.axis('equal')
plt.savefig("plots/"+plot2Name)


print "ALMOST DONE"