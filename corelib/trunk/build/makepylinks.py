
import sys
import os

args = sys.argv[1:]

for arg in args:
    os.chdir(arg)
    cmd = "ln -s lib_%s.so _%s.so" % (arg,arg)
    os.system(cmd)
    os.chdir("..")
    
    print cmd
    
    
