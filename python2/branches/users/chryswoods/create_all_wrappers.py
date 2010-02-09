
import os

dirs = [ "Base", \
         "CAS", \
         "Cluster", \
         #"DB", \
         #"FF", \
         "ID", \
         #"IO", \
         #"MM", \
         "Maths", \
         #"Mol", \
         #"Move", \
         "Sec", \
         "Siren", \
         #"Spier", \
         #"Squire", \
         #"System", \
         "Units", \
         "Vol" ]

def create_wrappers(dir): 
    os.chdir(dir)
    os.system("python ../AutoGenerate/create_wrappers.py")
    os.chdir("..")

for dir in dirs:
    create_wrappers(dir)

#os.chdir("Qt")
#os.system("python create_qt_wrappers.py")
#os.chdir("..")

