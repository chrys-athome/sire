###################################
##                               ##
##  Sire Configuration File      ##
##                               ##
##  This file is used to         ##
##  configure the compilation    ##
##  of Sire. Edit this file      ##
##  to configure for your system ##
##                               ##
###################################

###########################################
##
## Basic configuration
##
###########################################

#Which directory do you wish to install Sire (and its components) in? 
# Note that you cannot move Sire once it has
# been installed. You will instead need to change this
# value and rerun 'make install' - this will however be quick
# as the installation path is not hard-coded, but is rather 
# written into the wrapper scripts that run sire. You can 
# override this value by setting 'sire.install' on the qmake
# command line

installdir = /home/chris/local

#Uncomment this if you wish to compile for debugging
#CONFIG += debug

#Comment this line if you want a more verbose build output
#CONFIG += silent

#Absolute path to the python executable that you wish to use 
#as the python shell in Sire - note that this should be python2.3
#or greater

python = /usr/bin/python

#optimisation flags - first for compiling C code (sire.compile.cflags)
#                     then for C++ code (sire.compile.cxxflags)

sire.compile.cflags = -O2
sire.compile.cxxflags = $$sire.compile.cflags

#debugging flags - these are the flags that are used when compiling
#for debugging

sire.debug.cflags = -O2 -g -gdb
sire.debug.cxxflags = $$sire.compile.cxxflags

###########################################
##
## Advanced configuration
##
###########################################

#Information about python - this is obtained automatically
#from the python executable specified in $$python, but you
#can override it here by uncommenting and specifying below
#python.headers = /usr/include/python2.4
#python.libs = -lpython2.4 -lm -L/usr/lib/python2.4/config

#if you are statically linking the python interpreter than you
#need to be careful with the link flags used - see the "linking
#requirements" section of the python manual
# (http://docs.python.org/ext/link-reqs.html)

#If you are not specifying the python flags, then you can
#uncomment the below line to link the python interpreter 
#statically to sire
#python.linkstatic = true

#Information about boost - sire needs boost >= 1.31
boost.headers =
boost.libs = -lboost_python

#Information about the GNU Scientific Library - need GSL >= 0.7
gsl.headers =
gsl.libs = -lgsl -lgslcblas

# add any additional linking flags that you wish to use
# by uncommenting and specifying here
#sire.lflags = 

#uncomment the below line if you want to disable symbol hiding
#sire.no_symbol_hiding = 1

# this gets the root directory in which sire will be installed
# This will be equal to 'installdir' if that was set above, or
# to 'sire.install' if that was set on the qmake command line,
# or to $HOME/sire-$version if neither or these is set
isEmpty(sire.install){
    isEmpty(installdir) {
	    sire.install = "$$(HOME)/sire-$${sire.version}"
	} else {
		sire.install = $$installdir
	}
}

#by default the binaries will go in install/bin, libraries in
#install/lib and extra files in install/share - you
#can change this by changing these values...

#directory in which to install binaries
isEmpty(sire.install.bin) {
	sire.install.bin = $$joinPath( $$sire.install, bin )
}

#directory in which to install libraries
isEmpty(sire.install.lib) {
	sire.install.lib = $$joinPath( $$sire.install, lib )
}

#directory in which to install additional data - used
#as the root of the python module and plugin directories
isEmpty(sire.install.share) {
	sire.install.share = $$joinPath( $$sire.install, share )
}

#directory in which to install the python code
isEmpty(sire.install.python) {
	sire.install.python = $$joinPath( $$sire.install.share, python )
}

#directory in which to install any plugins
isEmpty(sire.install.plugin) {
	sire.install.plugin = $$joinPath( $$sire.install.share, plugins )
}

