#########
##
## QMake include file that sets the project wide variables
## used during compilation
##
## Christopher Woods
##
##

#all compilation occurs in hidden directories
MOC_DIR = .moc
OBJECTS_DIR = .obj_$${QMAKE_CC}_$${QMAKE_CXX}
UIC_DIR = .uic

#show compiler warnings, use prl for library linking
CONFIG += warn_on create_prl link_prl

#The version of Sire
sire.version = 0.0.1

#The sire authors
sire.authors = Christopher Woods

#This function joins the two arguments together into 
#a single path, using the correct separator for the operating system
# (e.g. $$joinPath(dirname,filename) will return dirname/filename on unix/mac,
# and dirname\filename on windows)
defineReplace(joinPath) {
    root = $$1
    file = $$2

    sep = /
    windows{ sep = \\ }
    
    path = $$1 $$2
    
    path = $$join(path,$$sep)
    return($$path)
}

#This function returns the absolute path to the file 'file' in
#the directory 'dir'
defineReplace(absPath) {
    dir = $$1
    file = $$2
    
    sep = /
    pwd = $$(PWD)
    
    window{ sep = \\ }
    
    path = $$pwd $$dir $$file
    path = $$join(path,$$sep)
    return($$path)
}

#This function is used to write the command scripts used to run the programs
defineReplace(writeCmdFile) {
    exefile = $$joinPath($$sire.compile.bin,$$1)
    exename = $$2
    
    runwriter = $$joinPath($$sire.build, runwriter.py)
    
    output = $$system($$python $$runwriter $$exefile $$exename $$sire.install.bin \
                      $$sire.install.lib $$sire.install.share $$sire.install.python \
                      $$sire.install.plugin)
    
    return($$exefile)
}      

#include the 'configure.pri' file, that includes all global, user-set
#configuration options
!include($$sire.config): error(Cannot find configuration file '$$sire.config')

#now use build/python-config to get the flags necessary to link 
#to the python interpreter
python_config = $$python $$absPath($$sire.build, python-config)

isEmpty(python.headers) {
    python.headers = $$system($$python_config --cflags)
}

isEmpty(python.libs) {
    isEmpty(python.linkstatic) {
        python.libs = $$system($$python_config --lflags)
    } else {
        python.libs = $$system($$python_config --static)
    }
}    

#no Qt components are used by default
QT = 

#set the library version numbers
VERSION = $$sire.version

#set the location of the precompiled headers
sire.precompiled_headers = $$joinPath($$sire.build, static_headers.h)

#now set the temporary locations of the apps/libs/wrappers 
#when they are compiled
sire.compile.lib = $$joinPath($$sire.root, .lib)
sire.compile.bin = $$joinPath($$sire.root, .bin)
sire.compile.wrappers = $$joinPath($$sire.root, .wrappers)

#all of the header files for the libraries are rooted in the src/libs directory
sire.src = $$joinPath($$sire.root, src)
sire.src.libs = $$joinPath($$sire.src,libs)
INCLUDEPATH += $$sire.src.libs

#ensure that the linker can find libraries installed in sire.compile.libs
LIBS += -L$$sire.compile.lib

#now include any local configuration for the project - local configuration is 
#contained in this file, rather than the root .pro file, as kdevelop has a habit
#of rewriting .pro files in the project...
include(local.pri)

#set the compiler flags from those specified in the configuration file
sire.cflags.release = $$sire.compile.cflags
sire.cflags.debug = $$sire.debug.cflags

sire.cxxflags.release = $$sire.compile.cxxflags
sire.cxxflags.debug = $$sire.debug.cxxflags

sire.install.python.root = $$joinPath($$sire.install.python,Sire)

#if this is a library, then activate global library options
contains(TEMPLATE,lib) {
  isEmpty(python.wrapper) {
      CONFIG += dll
      DESTDIR = $$sire.compile.lib
      target.path = $$sire.install.lib
      INSTALLS += target
  } else {
      #this is a python wrapper plugin
      CONFIG += plugin
      DESTDIR = $$sire.compile.wrappers

      #add the wrappers directory to the include path
      INCLUDEPATH += $$joinPath($$sire.src.libs,PyWrap)
      
      wrapperdir = $$joinPath($$sire.install.python.root,$$python.wrapper)
      target.path = $$joinPath($$sire.install.python.root,$$python.wrapper)
      target.target = $$python_wrapper
      INSTALLS += target
  }
} else: contains(TEMPLATE,app) {
      #if this is an application, then activate global application options
      DESTDIR = $$sire.compile.bin
      target.path = $$sire.install.bin
      INSTALLS += target
}

#get the compiler flags needed to use symbol hiding. You can
#disable symbol hiding by setting sire.no_symbol_hiding to any value
isEmpty(sire.no_symbol_hiding) {

	isEmpty( QMAKE_CFLAGS_HIDESYMS ) {
		sire.cflags.hidesyms = -DSIRE_NO_VISIBILITY_AVAILABLE
		sire.cxxflags.hidesyms = -DSIRE_NO_VISIBILITY_AVAILABLE
	} else {
		#message(Building $$TARGET using symbol hiding)
		sire.cflags.hidesyms = $$QMAKE_CFLAGS_HIDESYMS
		sire.cxxflags.hidesyms = $$QMAKE_CXXFLAGS_HIDESYMS
	}
} else {
	#message(Symbol hiding has been disabled for $$TARGET)
	sire.cflags.hidesyms = -DSIRE_NO_VISIBILITY_AVAILABLE
	sire.cxxflags.hidesyms = -DSIRE_NO_VISIBILITY_AVAILABLE
}

#add the global C and C++ compiler flags
QMAKE_CFLAGS_RELEASE += $$sire.cflags.release $$sire.cflags.hidesyms
QMAKE_CXXFLAGS_RELEASE += $$sire.cxxflags.release $$sire.cxxflags.hidesyms

QMAKE_CFLAGS_DEBUG += $$sire.cflags.debug $$sire.cflags.hidesyms
QMAKE_CXXFLAGS_DEBUG += $$sire.cxxflags.debug $$sire.cxxflags.hidesyms

#the local.pri file can define a "sire.depends" that lists the Sire libraries that
#it depends on. Loop through all of these and add them to the link flags
for(sirelib,sire.depends) {
	LIBS += -l$${sirelib}
	INCLUDEPATH += $$joinPath($$sire.src.libs,$$sirelib)
}

#finally, get rid of strict-aliasing warnings for gcc4 - they are annoying
#as QHash/QSet trigger them in nearly every source file!!!
contains(QMAKE_CC,gcc) {
        QMAKE_CFLAGS_WARN_ON = $$QMAKE_CFLAGS_WARN_ON -Wno-strict-aliasing
}

contains(QMAKE_CXX,g++) {
        QMAKE_CXXFLAGS_WARN_ON = $$QMAKE_CXXFLAGS_WARN_ON -Wno-strict-aliasing
}

