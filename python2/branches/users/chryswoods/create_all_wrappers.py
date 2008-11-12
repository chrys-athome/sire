
import os

dirs = [ "Base", \
         "CAS", \
         "Cluster", \
         "DB", \
         "FF", \
         "ID", \
         "IO", \
         "MM", \
         "MPI", \
         "Maths", \
         "Mol", \
         "Move", \
         "Squire", \
         "Stream", \
         "System", \
         "Units", \
         "Vol" ]

def create_wrappers(dir): 
    os.chdir(dir)
    os.system("python ../AutoGenerate/create_wrappers.py")
    os.chdir("..")

for dir in dirs:
    create_wrappers(dir)

