# TODO: read the file params

import os.path
import re


def get_threshold_dynamic(new_val, samples_object, debug):
	samples_object[3] -= samples_object[0][(samples_object[2]+samples_object[1]) % samples_object[1]]		# Remove last sample
	tmp1 = samples_object[0][(samples_object[2]+samples_object[1]) % samples_object[1]]
	samples_object[0][samples_object[2]] = new_val															# Add new val
	samples_object[3] += new_val																			# Add new val to sum
	if (debug == 1):
		for i in range(len(samples_object[0])):
			debug_log.write(str(samples_object[0][i]))
			debug_log.write("\t")
		debug_log.write(str(samples_object[2]))
		debug_log.write("\t")
		debug_log.write(str(samples_object[3]))
		debug_log.write("\t")
		debug_log.write(str(int(samples_object[3] / samples_object[1])))
		debug_log.write("\t")
		debug_log.write(str(tmp1))
		debug_log.write("\t")
		debug_log.write(str(new_val))
		debug_log.write("\n")
	samples_object[2] = (samples_object[2] + 1) % samples_object[1]											# Shift cursor
	return (samples_object[3] / samples_object[1], samples_object)


def map_range(x, in_min, in_max, out_min, out_max):
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min


debug_mode = int(input("Debug mode (0|1) - (no|yes):\n> "))
if (debug_mode != 0 and debug_mode != 1):
	print("Wrong")
	quit(-1)

filename_in = input("File:\n> ")	# Request filename
dynamic_threshold = int(input("Threshold (0|1) - (static threshold|dynamic threshold):\n> "))


if (dynamic_threshold == 0):
	threshold_static = int(input("Static threshold level:\n> "))
elif (dynamic_threshold == 1):
	sample_val_array_length = int(input("Dynamic threshold history length:\n> "))
	sample_val_array = [0] * sample_val_array_length
	sample_val_array_index = 0
	sample_var_array_sum = 0
	sample_var_array_block = [sample_val_array, sample_val_array_length, sample_val_array_index, sample_var_array_sum]
else:
	print("Wrong")
	quit(-2)

# Get names
filename_in_no_suffix = re.sub("\..+?$", "", filename_in)
filename_in_suffix = "." + re.sub("^.*\.", "", filename_in)
filename_out = filename_in_no_suffix + "_compressed" + filename_in_suffix
if(filename_in_suffix.lower() != ".wav"):
	print("Not a wav")
	quit(-3)

# Open file
file_in = open(filename_in, "rb")
try:
	file_out = open(filename_out, "xb")
except FileExistsError:
	file_out = open(filename_out, "wb")
if (debug_mode == 1):
	try:
		debug_log = open("wavToNAT1BIT_process.log", "x")
	except FileExistsError:
		debug_log = open("wavToNAT1BIT_process.log", "w")
		
	for i in range(sample_val_array_length):
		debug_log.write(str(i))
		debug_log.write("\t")
	debug_log.write("idx\tsum\tavg\tsub\tadd\n")

progress_current = 0
progress_last = -1
# Copy
for cursorI in range(os.path.getsize(filename_in)):											# Go through the whole file
	progress_current = int(map_range(cursorI, 0, os.path.getsize(filename_in) - 1, 0, 100))				# Get progress
	if (progress_current != progress_last):															# Progress changed
		progress_last = progress_current															# Update change detection
		print("Progress: %02i%%" % progress_current)												# Print progress
	readByte = file_in.read(1)																	# Read
	if (debug_mode == 1):																		# If debug
		debug_log.write("READ: 0x%02x\t" % int.from_bytes(readByte, "little"))						# Print read data
	readByte = int.from_bytes(readByte, "little")												# Convert to int
	if(cursorI < 44):																			# Pos - Header
		writeByte = readByte																		# Copy
		if (cursorI == 0x18):																		# If at smaple rate
			wavfile.seek(0x18)																			#
			sampleRate = readBytes(wavfile, 4)															# Read sample rate
			wavfile.seek(0x19)																			#
		if (cursorI == 0x20):																		# If at bytes per sample
			wavfile.seek(0x20)																			#
			bytesPerSample = readBytes(wavfile, 2)														# Read bytes persample
			wavfile.seek(0x21)																			#
		if (cursorI == 0x24):																		# If at data tag
			wavfile.seek(0x24)																			#
			if (wavfile.read(4) != b'data'):															# No data tag
				print("No data tag at 0x24")																#
				file_in.close()																				#
				file_out.close()																			#
				debug_log.close()																			#
				quit(-3)																					#
			wavfile.seek(0x25)																			#
		if (cursorI == 0x28):																		# Read the size of the samples
			wavfile.seek(0x28)																			#
			sampledDataSize = readBytes(wavfile, 4)														# Read the size of the samples
			wavfile.seek(0x29)																			#
		if (cursorI == 43):
			print("Sample rate: " + str(sampleRate))
			print("Bytes per sample: " + str(bytesPerSample))
			print("Sampled data size: " + str(sampledDataSize))
	elif (cursorI >= 44 and cursorI < 44 + sampledDataSize):							# Pos - Data
		if (dynamic_threshold == 0):																# Threshold mode - static
			threshold = threshold_static																# Static threshold
		elif (dynamic_threshold == 1):																# Threshold mode - dynamic
			# if (readByte <= int(0xff / 2)):																# Filter positive only
			# 	readByte = int(0xff / 2)																	# Else 0xff/2
			get_threshold_out = get_threshold_dynamic(readByte, sample_var_array_block, debug_mode)					# Get threshold
			sample_var_array_block = get_threshold_out[1]												# Get threshold 2
			threshold = int(get_threshold_out[0])														# Get threshold 3
		if (debug_mode == 1): 																		 # If debug
			debug_log.write("THRE: 0x%02x\t" % threshold)												# Print threshold
		if (readByte > threshold):																	# Compress
			writeByte = 0xff																			# High
		else:																						# Compress - Else
			writeByte = 0																				# Low
	else:																						# Pos - else
		writeByte = readByte																		# Copy
	writeByte = int(writeByte).to_bytes(1, "little")											# Convert to byte
	file_out.write(writeByte)																	# Write
	if (debug_mode == 1): 																		# If debug
		debug_log.write("WRITE: 0x%02x\n" % int.from_bytes(writeByte, "little"))					# Print written data

print("DONE")
# Close file
file_in.close()
file_out.close()
debug_log.close()
