import os
import re

path = re.sub("/V2.*", "", re.sub("\\\\", "/", str(os.path.abspath(os.getcwd()))))
f = open("LibFilepathFix.h", "w")
f.write("#include \"" + str(path) + "/V2/Libs/PinDefs.h\"\n")
f.write("#include \"" + str(path) + "/V2/Libs/Driver.h\"\n")
f.write("#include \"" + str(path) + "/V2/Libs/ToFrame.h\"")
f.close()