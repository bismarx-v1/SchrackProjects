import os
import math
bytes = 1  # Switch between 8bit and 16bit

filenameIn = "file_yoink_8kHz.wav"
filenameOut = "file_yoink_8kHz_Compressed.wav"
fileIn = open(filenameIn, "rb")
try:
    fileOut = open(filenameOut, "wb")
except:
    fileOut = open(filenameOut, "xb")

for i in range(43):
    fileOut.write(fileIn.read(1))

if(bytes == 1):
    for i in range(os.path.getsize(filenameIn) - 43):
        readByte = fileIn.read(1)
        readByte = int.from_bytes(readByte, "little")
        #if(readByte > 0xBD):
        if(readByte > 0x96):
            readByte = 0xff
        else:
            readByte = 0
        readByte = readByte.to_bytes(1, "little")
        fileOut.write(readByte)
                  
elif(bytes == 2):
    for i in range(math.floor((os.path.getsize(filenameIn) - 43) / 2)):
        readByte = fileIn.read(2)
        readByte = int.from_bytes(readByte, "little")
        if(readByte > 0xBFFD):
            readByte = 0xffff
        else:
            readByte = 0
        readByte = readByte.to_bytes(2, "little")
        fileOut.write(readByte)


fileOut.close()
fileIn.close()
print("DONE")
