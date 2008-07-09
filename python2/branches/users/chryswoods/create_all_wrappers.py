
import os

dirs = [ "CAS", \
         "FF", \
         "ID", \
         "IO", \
         "MM", \
         "Maths", \
         "Mol", \
         "Units", \
         "Vol" ]

def create_wrappers(dir): 
    os.chdir(dir)
    os.system("python ../AutoGenerate/create_wrappers.py")
    os.chdir("..")

for dir in dirs:
    create_wrappers(dir)

