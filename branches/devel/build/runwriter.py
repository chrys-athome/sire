##################################
##
## This class is used to write
## small scripts that are used
## to set the library paths
## to run applications...
##
## Christopher Woods

import os

class RunWriter:
    def __init__(self):
        self._dirs = {}
        
    def setDir(self,typ,dirname):
        self._dirs[typ] = dirname

    def write(self,filename,exe):
        fle = open(filename,"w")
        
        fle.write("#!/bin/env bash\n\n")
        fle.write("#Auto-generated script to run %s\n\n" % exe)
        
        for dirname in self._dirs:
            fle.write("export %s=%s\n" % (dirname,self._dirs[dirname]))
            
        fle.write("\nexport LD_LIBRARY_PATH=$SIRE_LIB_DIR:$LD_LIBRARY_PATH\n")
        fle.write("export PYTHONPATH=$SIRE_PYTHON_DIR/:$PYTHONPATH\n")

        fle.write("if [[ $1 = \"gdb\" ]]\n")
        fle.write("then\n")
        fle.write("  shift\n")
        fle.write("  echo \"'$@'\"\n")
        fle.write("  gdb $SIRE_BIN_DIR/%s\n" % exe)
        fle.write("elif [[ $1 = \"ddd\" ]]\n")
        fle.write("then\n")
        fle.write("  shift\n")
        fle.write("  echo \"'$@'\"\n")
        fle.write("  ddd $SIRE_BIN_DIR/%s\n" % exe)
        fle.write("else\n")
        fle.write("  $SIRE_BIN_DIR/%s $@\n" % exe)
        fle.write("fi\n\n")
        fle.close()
        os.system("chmod a+x %s" % filename)
        
if (__name__ == "__main__"):
    
    import sys
    import string
    
    #get the cmd line arguments - these are;
    
    # (1) name of file to write
    # (2) Name of executable
    # (3) executable install directory
    # (4) library install dir
    # (5) share install dir
    # (6) python modules install dir
    # (7) plugin install dir
    
    cmdfile = sys.argv[1]
    exename = sys.argv[2]
    bindir = sys.argv[3]
    libdir = sys.argv[4]
    sharedir = sys.argv[5]
    pythondir = sys.argv[6]
    plugindir = sys.argv[7]
    
    rwtr = RunWriter()
    
    rwtr.setDir("SIRE_BIN_DIR",bindir)
    rwtr.setDir("SIRE_LIB_DIR",libdir)
    rwtr.setDir("SIRE_SHARE_DIR",sharedir)
    rwtr.setDir("SIRE_PYTHON_DIR",pythondir)
    rwtr.setDir("SIRE_PLUGIN_DIR",plugindir)
    
    rwtr.write(cmdfile, exename)
    
        
