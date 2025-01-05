import os
import math

filenameIn = "file_Blue_8kHz.wav"
filenameOut = "file_Blue_8kHz_Compressed.wav"
fileIn = open(filenameIn, "rb")
try:
    fileOut = open(filenameOut, "wb")
except:
    fileOut = open(filenameOut, "xb")

for i in range(43):
    fileOut.write(fileIn.read(1))

lastFewBytes = [0] * 16
lastFewBytesIndex = 0
threshold = 0

for i in range(os.path.getsize(filenameIn) - 43):
    readByte = fileIn.read(1)
    readByte = int.from_bytes(readByte, "little")
    lastFewBytes[lastFewBytesIndex] = readByte
    lastFewBytesIndex = (lastFewBytesIndex + 1) % len(lastFewBytes)
    
    for i2 in range(len(lastFewBytes)):
        threshold += lastFewBytes[i2]
    threshold = round(threshold / len(lastFewBytes))
    
    if(readByte > threshold):
        readByte = 0xff
    else:
        readByte = 0
    readByte = readByte.to_bytes(1, "little")
    fileOut.write(readByte)


fileOut.close()
fileIn.close()
print("DONE")
