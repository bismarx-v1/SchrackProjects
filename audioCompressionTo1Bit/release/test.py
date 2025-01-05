
def intToBytes(num, nOfBytes):
	byteArray = []
	for index in range(nOfBytes):
		byteArray.append((num >> (nOfBytes - 1 - index) * 8) & 0xff)
	return bytes(byteArray)

def bytesToInt(bytes, nOfBytes):
	readBytes = 0
	for index in range(nOfBytes):
		print(index)
		readBytes += bytes[index] << ((nOfByte - index) * 8)
	return readBytes


try:
	file = open("test.txt", "xb")
except:
	file = open("test.txt", "wb")

num = 1194249
#file.write(intToBytes(num, 4))
file.write(b'\0b01100101')
file.write(b'\0b0')
file.write(b'\0b0')
file.write(b'\0b0')
file.close()

file = open("test.txt", "rb")
readBytes = file.read(4)
print(readBytes)
#print(bytesToInt(readBytes, 4))
file.close()
