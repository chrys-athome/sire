# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: .
# Target is a subdirs project 

TEMPLATE = subdirs
sire.root = .. # AUTO_GENERATED_LINE
sire.build = $$sire.root/build # AUTO_GENERATED_LINE
sire.config = $$sire.root/configure.pri # AUTO_GENERATED_LINE
sire.global = $$sire.build/global.pri # AUTO_GENERATED_LINE
!include($$sire.global): error(Cannot find global configure file!) # AUTO_GENERATED_LINE
SUBDIRS += libs \
           apps
           
