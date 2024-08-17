import re
import cv2
import numpy as np

TestPrints = ["ShrackCharSet.png", "TestFrameHewwoCol3.png", "TestFrameHelloColGrtd.png", "TestFrameBarcodeH.png", "TestFrameBarcodeHInv.png", "TestFrameBarcodeW.png", "TestFrameBarcodeWInv.png", "TestFrameCheckerboard.png", "TestFrameCheckerboardInv.png", "SmallTestFrame.png"]
Offset = -1

Img = cv2.imread(str(TestPrints[Offset]), -1)

Out=[]

print("const uint8_t " + str(re.sub("\.([^.]*)$", "", str(TestPrints[Offset]))) + "[" + str(len(Img)*len(Img[0])) + "] = {", end="")
#print("\n", end="")
for y in range(len(Img)):
	for x in range(len(Img[0])):
		if(Img[y][x][0].astype(int)+Img[y][x][1].astype(int)+Img[y][x][2].astype(int) >= 384):
			Out.append(0)
			#print("░░", end="")
			print("0", end="")
		else:
			Out.append(1)
			#print("▓▓", end="")
			print("1", end="")
		print(", ", end="")
	#print("\n", end="")
print("\b\b", end="")
print("};")
print("Items: " + str(len(Img)*len(Img[0])), end="")