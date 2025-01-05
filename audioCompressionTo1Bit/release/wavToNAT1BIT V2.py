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


lastFewBytesIndex = 0
lastFewBytes = [0] * 16

def getThreshold(gt_lastFewBytes, gt_lastFewBytesIndex):
	gt_lastFewBytes[gt_lastFewBytesIndex] = readByte
	gt_lastFewBytesIndex = (gt_lastFewBytesIndex + 1) % len(gt_lastFewBytes)
	threshold = 0
	for i2 in range(len(gt_lastFewBytes)):
		threshold += gt_lastFewBytes[i2]
	threshold = round(threshold / len(gt_lastFewBytes))
	return (threshold, gt_lastFewBytes, gt_lastFewBytesIndex)



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
nat1bitfilePreviewName = wavfileName + ".NAT1BIT_Preview.wav"
nat1bitfileName = wavfileName + ".NAT1BIT"

# check if the file is wav
if(re.search(".wav$", wavfileName) == None):
	print("Not a wav")
	quit(-1)

# try to create the out file
try:
	nat1bitfilePreview = open(nat1bitfilePreviewName, "xb")	# create file
# out file exists
except:
	print(nat1bitfilePreviewName + " exists")
	quit(-2)

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
	nat1bitfilePreview.close()
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
	nat1bitfilePreview.write(wavfile.read(1))
	
lastFewBytes = [0] * 16
lastFewBytesIndex = 0

# calculate how long will the data in the out file be
nat1bitfileSizeFullBytes = sampledDataSize // 8
nat1bitfileSizePartialByteBits = sampledDataSize % 8
print("Full bytes: " + str(nat1bitfileSizeFullBytes))
print("Bits in partial byte: " + str(nat1bitfileSizePartialByteBits))

# header to out file
nat1bitfile.write(nat1bitfileSizeFullBytes.to_bytes(4, "little"))	# full bytes
nat1bitfile.write(nat1bitfileSizePartialByteBits.to_bytes(1, "little"))	# partial byte
nat1bitfile.write(sampleRate.to_bytes(4, "little"))	# sample rate in Hz

wavfilePos = wavfile.tell()
# write the preview
for i in range(sampledDataSize): # same as the for i2 part just before
	# read sample
	readByte = readBytes(wavfile, bytesPerSample)
	
	# get threshold
	
	getThresholdOut = getThreshold(lastFewBytes, lastFewBytesIndex)
	threshold = getThresholdOut[0]
	lastFewBytes = getThresholdOut[1]
	lastFewBytesIndex = getThresholdOut[2]
	
	# use threshold
	#print(readByte, end="\t")
	#print(threshold, end="\t")
	if (readByte > threshold):
		writeVal = (1 << (bytesPerSample * 8)) - 1
		#print(1)
	else:
		writeVal = 0
		#print(0)
	nat1bitfilePreview.write(writeVal.to_bytes(1, "little", signed=False))
	
for i in range(os.path.getsize(wavfileName) - wavfile.tell()):
	nat1bitfilePreview.write(wavfile.read(1))

# read again bur write to the out file
wavfile.seek(wavfilePos)

# reading the samples
for i in range(nat1bitfileSizeFullBytes):
	writeByte = 0
	for i2 in range(8):  # for 1 byte of the NAT1BIT file
		# read sample
		readByte = readBytes(wavfile, bytesPerSample)
		# get bit using threshold
		getThresholdOut = getThreshold(lastFewBytes, lastFewBytesIndex)
		threshold = getThresholdOut[0]
		lastFewBytes = getThresholdOut[1]
		lastFewBytesIndex = getThresholdOut[2]
		
		if (readByte > threshold):
			writeByte += 1 << i2
		else:
			writeByte += 0 << i2  # does nothing
	
	nat1bitfile.write(writeByte.to_bytes(1, "big"))  # write full byte to file - 8 samples

# the partial byte
for i in range(nat1bitfileSizePartialByteBits):  # same as the for i2 part just before
	# read sample
	readByte = readBytes(wavfile, bytesPerSample)
	writeByte = 0
	# get bit using threshold
	getThresholdOut = getThreshold(lastFewBytes, lastFewBytesIndex)
	threshold = getThresholdOut[0]
	lastFewBytes = getThresholdOut[1]
	lastFewBytesIndex = getThresholdOut[2]
	
	if (readByte > threshold):
		writeByte += 1 << i
	else:
		writeByte += 0 << i  # does nothing

nat1bitfile.write(writeByte.to_bytes(1, "big"))  # write the partial byte to file - < 8 samples

wavfile.close()
nat1bitfilePreview.close()
nat1bitfile.close()

print("DONE")
quit(0)
