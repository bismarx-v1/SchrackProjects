"""
.NAT1BIT
0x00					- 4 bytes		- sizeFullBytes		- count of full bytes (8 samples), written in little endian
0x04					- 1 byte		- sizePartialBytes	- number of bits in the partial byte (additional byte with remaining samples), written in little endian
0x05					- 4 bytes		- sampleRate		- sample rate in Hz written in little endian
0x09					- sizeFullBytes	- <samples>			- in groups of 8 bits, count determined by the sizeFullBytes
0x<sizeFullBytes + 9>	- < 1 byte		- <rem samples>		- max 7 samples that didn't fill a full byte
"""
import os.path
import re


def readBytes(file, nOfBytes):
	ret = 0
	for index in range(nOfBytes):
		readByte = file.read(1)
		ret += int.from_bytes(readByte, "little") << (index * 8)
	return ret





# ask for in file
# expected a .wav file
wavfileName = input("In file: ")

# create a name for the out file (just append .NAT1BIT)
nat1bitfileName = wavfileName + ".NAT1BIT_Preview.wav"

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

wavfile.seek(0)
for i in range(0x44):
	nat1bitfile.write(wavfile.read(1))
	
lastFewBytes = [0] * 16
lastFewBytesIndex = 0

# the partial byte
for i in range(sampledDataSize): # same as the for i2 part just before
	# read sample
	readByte = readBytes(wavfile, bytesPerSample)
	
	# get threshold
	lastFewBytes[lastFewBytesIndex] = readByte
	lastFewBytesIndex = (lastFewBytesIndex + 1) % len(lastFewBytes)
	threshold = 0
	for i2 in range(len(lastFewBytes)):
		threshold += lastFewBytes[i2]
	threshold = round(threshold / len(lastFewBytes))
	
	# use threshold
	print(readByte, end="\t")
	print(threshold, end="\t")
	if (readByte > threshold):
		writeVal = (1 << (bytesPerSample * 8)) - 1
		print(1)
	else:
		writeVal = 0
		print(0)
	nat1bitfile.write(writeVal.to_bytes(1, "little"))
	
for i in range(os.path.getsize(wavfileName) - wavfile.tell()):
	nat1bitfile.write(wavfile.read(1))

wavfile.close()
nat1bitfile.close()

print("DONE")
quit(0)
