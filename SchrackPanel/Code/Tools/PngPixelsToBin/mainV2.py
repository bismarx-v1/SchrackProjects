import re
import cv2
import numpy as np

#BGR not RGB
Input = "CharSet.png"

Img = cv2.imread(Input, -1)
Out = []
Out2 = []
TempWidth = 0
for x in range(len(Img[0])):
	for y in range(len(Img)):
		if(Img[y][x][0].astype(int) == 255):		# Decode blue channel
			Out.append(1)
		else:
			Out.append(0)
	
	if x % 6 == 5:
		for y in range(len(Img)):
			if (Img[y][x][2].astype(int) == 255):		# Decode red channel
				TempWidth = TempWidth + 2**y
		Out2.append(TempWidth)
		TempWidth = 0

print("const uint8_t CharSet[" + str(len(Out)) + "] = {", end="")
for i in range(len(Out)):
	if(i == len(Out)-1):
		print(str(Out[i]), end="")
	else:
		print(str(Out[i]), end=", ")
	
print("};\n")

print("const uint8_t CharSetWidths[" + str(len(Out2)) + "] = {", end="")
for i in range(len(Out2)):
	if (i == len(Out2) - 1):
		print(str(Out2[i]), end="")
	else:
		print(str(Out2[i]), end=", ")

print("};")

quit()		# dunno what's past this. probs ok to delete

'''	#img format convert
Img2 = cv2.imread("SOmething.png", -1)
for i in range(90):
	for y in range(7):
		for x in range(6):
			Img[y][x+i*6][0] = i%2*20+200
			Img[y][x+i*6][1] = i%2*20+200
			Img[y][x+i*6][2] = i%2*20+200
			if(Img2[y+i*7][x][0] < 200):
				Img[y][x+i*6] = Img2[y+i*7][x]
cv2.imwrite(Input, Img)
cv2.imwrite("CharSet.png", Img)
'''


quit()

Out=[]

print("const uint8_t " + str(re.sub("\.([^.]*)$", "", str(TestPrints[Offset]))) + "[" + str(len(Img)*len(Img[0])) + "] = {", end="")
#print("\n", end="")
if(TestPrints[Offset] == "ShrackCharSet.png"):
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
else:
	for x in range(len(Img[0])):
		for y in range(len(Img)):
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
print("Items: " + str(len(Img)*len(Img[0])), end="\n")

quit()
for y in range(len(Img)):
	for x in range(len(Img[0])):
		if (TestPrints[Offset] == "ShrackCharSet.png"):
			print(Out[x+y*5], end="")
		else:
			print(Out[x*7+y], end="")
	print("\n", end="")
