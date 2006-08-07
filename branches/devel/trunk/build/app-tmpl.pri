#########
##
## QMake include file that sets the project wide variables
## for all applications that will be compiled
##
## Christopher Woods
##
##

#Set the destination directory for compiled binaries
DESTDIR = $$SIRE_BINDIR

target.path = $$SIRE_INSTALL_BINDIR
INSTALLS += target
