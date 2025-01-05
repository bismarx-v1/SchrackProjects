"""
.NAT1BIT
0x00					- 4 bytes		- sizeFullBytes		- count of full bytes (8 samples), written in little endian
0x04					- 1 byte		- sizePartialBytes	- number of bits in the partial byte (additional byte with remaining samples), written in little endian
0x05					- 4 bytes		- sampleRate		- sample rate in Hz written in little endian
0x09					- sizeFullBytes	- <samples>			- in groups of 8 bits, count determined by the sizeFullBytes
0x<sizeFullBytes + 9>	- < 1 byte		- <rem samples>		- max 7 samples that didn't fill a full byte
"""

import re


def readBytes(file, nOfBytes):
	ret = 0
	for index in range(nOfBytes):
		readByte = file.read(1)
		ret += int.from_bytes(readByte, "little") << (index * 8)
	return ret


lastFewBytes = [0] * 16
lastFewBytesIndex = 0
def getThresholdFromAvg16():
	ret = 0
	for index in range(len(lastFewBytes)):
		ret += lastFewBytes[index]
	return round(ret / len(lastFewBytes))


def getThreshold():
	#return getThresholdFromAvg16()
	return 190


# ask for in file
# expected a .wav file
wavfileName = input("In file: ")

# create a name for the out file (just append .NAT1BIT)
nat1bitfileName = wavfileName + ".NAT1BIT"

# check if the file is wav
if(re.search(".wav$", wavfileName) == None):
	print("Not a wav")
	quit(-1)

# try to create the out file
try:
	nat1bitfile = open(nat1bitfileName, "xb")	# create file
# out file exists
except:
	print(nat1bitfileName + " exists")
	quit(-2)

# open the in file for reading as bytes
wavfile = open(wavfileName, "rb")

# read sample rate
wavfile.seek(0x18)
sampleRate = readBytes(wavfile, 4)

# read bytes per sample
wavfile.seek(0x20)
bytesPerSample = readBytes(wavfile, 2)

# read the data tag
wavfile.seek(0x24)
if(wavfile.read(4) != b'data'):
	print("No data tag at 0x24")
	wavfile.close()
	nat1bitfile.close()
	quit(-3)

# read the size of the samples
sampledDataSize = readBytes(wavfile, 4)

# print stuff about the loaded file
print("Sample rate: " + str(sampleRate))
print("Bytes per sample: " + str(bytesPerSample))
print("Sampled data size: " + str(sampledDataSize))

# calculate how long will the data in the out file be
nat1bitfileSizeFullBytes = sampledDataSize // 8
nat1bitfileSizePartialByteBits = sampledDataSize % 8

# header to out file
nat1bitfile.write(nat1bitfileSizeFullBytes.to_bytes(4, "little"))	# full bytes
nat1bitfile.write(nat1bitfileSizePartialByteBits.to_bytes(1, "little"))	# partial byte
nat1bitfile.write(sampleRate.to_bytes(4, "little"))	# sample rate in Hz

# reading the samples
for i in range(nat1bitfileSizeFullBytes):
	writeByte = 0
	for i2 in range(8):	# for 1 byte of the NAT1BIT file
		# read sample
		readByte = readBytes(wavfile, bytesPerSample)
		# get bit using threshold
		threshold = getThreshold()	# change this to change the threshold
		if(readByte > threshold):
			writeByte += 1 << i2
		else:
			writeByte += 0 << i2	# does nothing
	
	nat1bitfile.write(writeByte.to_bytes(1, "big"))	# write full byte to file - 8 samples

# the partial byte
for i in range(nat1bitfileSizePartialByteBits): # same as the for i2 part just before
	# read sample
	readByte = readBytes(wavfile, bytesPerSample)
	writeByte = 0
	# get bit using threshold
	threshold = getThreshold()  # change this to change the threshold
	if (readByte > threshold):
		writeByte += 1 << i
	else:
		writeByte += 0 << i  # does nothing

nat1bitfile.write(writeByte.to_bytes(1, "big"))  # write the partial byte to file - < 8 samples

wavfile.close()
nat1bitfile.close()

print("DONE")
quit(0)
