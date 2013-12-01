import matplotlib.pyplot as plt
import string
import json

dataFilePath = "plots/data.json"
dataFile = open(dataFilePath, "r")
data = json.load(dataFile)
dataFile.close()
print len(data)
i = 1
while i < len(data)+1:
    yLabel = ""
    if i == 1:
        yLabel = "Angular Velocity of flywheel"
    elif i == 2:
        yLabel = "Position of Slider "
    elif i == 3:
        yLabel = "Velocity of Slider"
    elif i == 4:
        yLabel = "xComponent of reaction force at flywheel"
    plotName = "Plot"+str(i)
    plot = data[plotName]
    xData = plot[0]
    print len(xData)
    yData = plot[1]
    plotFileName = plotName+".png"
    i+=1  
    plt.plot(xData,yData)
    plt.xlabel("Time")
    plt.ylabel(yLabel)
    plt.savefig("plots/"+plotName)
    plt.clf()
    

print "ALMOST DONE"