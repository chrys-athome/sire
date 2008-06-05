#########
##
## QMake include file that sets the project wide variables
## for all libraries that will be compiled
##
## Christopher Woods
##
##

#Set the destination directory for compiled libraries
DESTDIR = $$SIRE_LIBDIR

CONFIG += dll

target.path = $$SIRE_INSTALL_LIBDIR
INSTALLS += target
