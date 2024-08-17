import os
import re

path = re.sub("/Releases.*", "", re.sub("\\\\", "/", str(os.path.abspath(os.getcwd()))))
f = open("LibFilepathFix.h", "w")
f.write("#include \"" + str(path) + "/Releases/Libs/PinDefs.h\"\n")
f.write("#include \"" + str(path) + "/Releases/Libs/Driver.h\"\n")
f.write("#include \"" + str(path) + "/Releases/Libs/ToFrame.h\"")
f.close()
