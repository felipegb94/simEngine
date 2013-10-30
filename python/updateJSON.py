import os
import json
import requests
import sympy
import string

admFileName = raw_input("Enter adm file name: ")
if ".adm" not in admFileName:
    admFileName = admFileName+".adm"
admFilePath = "../../builds/simEngine/models/"+admFileName
admFile = open(admFilePath, "r")
data = json.load(admFile)
admFile.close()

constraints = data["constraints"]
funString = ""
dfunString = ""
ddfunString = ""
t = sympy.var("t")
data["constraints"][0]["dfun"] = "hello world"

counter = 0
for constraint in constraints:
    funString = constraint["fun"]
    funString = funString.replace('^','**')
    
    if(funString == "NONE"):
        dfunString = "0"
        ddfunString = "0"
    else:
        fun = sympy.sympify(funString)
        dfun = sympy.diff(fun,t)
        ddfun = sympy.diff(dfun,t)
        funString = str(fun)
        dfunString = str(dfun)
        ddfunString = str(ddfun)
        data["constraints"][counter]["dfun"] = dfunString
        data["constraints"][counter]["ddfun"] = ddfunString
        counter = counter +1

admFile = open(admFilePath, "w+")
admFile.write(json.dumps(data))
admFile.close()  
print "JSON Updated"     
print json.dumps(data)
        
        
    
    


# tmp = data["location"]
 #data["location"] = path
 #data["mode"] = "replay"

 #jsonFile = open("replayScript.json", "w+")
 #jsonFile.write(json.dumps(data))
 #jsonFile.close()
