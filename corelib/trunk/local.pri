#####################################
#
# This file contains all of the local
# configuration for the Sire project.
#
# This is not in sire.pro, as kdevelop
# has the nasty habit of overwriting it!
#
#

sire.python.files = python/*
sire.python.path = $$sire.install.python
INSTALLS += sire.python

message(       Sire Version $$sire.version)
message( (C) $$sire.authors )
message( )

message(Type 'make' to compile the source code contained in $$sire.src)

message( )

message(Configured to install files into $$sire.install)
message(Installing executables to $$sire.install.bin)
message(Installing libraries to $$sire.install.lib)
message(Installing plugins to $$sire.install.plugin)
message(Installing python files to $$sire.install.python)
message(Using C compiler $$QMAKE_CC and C++ compiler $$QMAKE_CXX)

message( )
message(If you wish to change compiler then edit the QMAKESPEC environmental)
message(variable and rerun qmake on all of the project files)
message( - this can be done by running 'python build/rerunqmake.py' )

