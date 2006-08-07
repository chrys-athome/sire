
import os
import string

#very unix-ish!

libfiles = os.listdir(".lib")
wrappers = os.listdir(".wrappers")

libs = []

def process(lib):
    lib = lib[0:lib.find(".so")]
    if (lib.startswith("lib")):
        lib = lib[3:]
        
    return "-l%s" % lib

for lib in libfiles:
    if lib.endswith(".so"):
        libs.append( process(lib) )
        
for lib in wrappers:
    if lib.endswith(".so"):
        libs.append( process(lib) )
        
#ok - we now have a list of all of the libraries
#in .lib and .wrappers

#add these to src/apps/testlink/local.pri
f = file("src/apps/testlink/local.pri", "a")
f.write("LIBS += %s" % string.join(libs))
f.close()

