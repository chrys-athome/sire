
import Sire.Config

from Sire.Base import *

import sys
import os

sire_root = getInstallDir()

sire_bindir = Sire.Config.binary_directory
sire_libdir = Sire.Config.library_directory

sire_bundle_libdir = getBundledLibDir()
sire_bundle_bindir = os.path.realpath("%s/../bin" % sire_bundle_libdir)

try:
    install_name_tool = findExe("install_name_tool").absoluteFilePath()
    otool = findExe("otool").absoluteFilePath()
except:
    print("Could not find 'install_name_tool'. If you are ")
    print("running this command on OS X, then please ensure ")
    print("that you have install XCode and all tools.")
    print("If you are not running this command on OS X, then ")
    print("you can safely ignore this warning as you do not ")
    print("need to run this program.")
    sys.exit(0)

def updateRPATH(file, library, exe=False):
    """Update the link information in 'file' so that the 
       absolute link to 'library' is replaced with an
       @rpath link"""

    realpath = os.path.realpath(file)

    is_writable = os.access(realpath, os.W_OK)

    if not is_writable:
        os.system("chmod u+w %s" % realpath)

    rpath_library = "@rpath/%s" % library.split("/")[-1]
    print("Changing %s to %s in file %s" % (library, rpath_library, file))

    cmd = "%s -change %s %s %s" % (install_name_tool, \
                                   library, rpath_library, file)

    print(cmd)
    os.system(cmd)

    if not is_writable:
        os.system("chmod u-w %s" % realpath)

external_libraries = {}

def scanLibrary(file, exe=False):
    """Scan the library to find out the paths to all of the libraries
       that this library links to. If any of these libraries are contained
       in sire.app, then make sure that these libraries are linked using
       @rpath rather than using an absolute file path"""
    print(".", end="", flush=True)
    lines = os.popen("%s -L %s" % (otool, file)).readlines()

    if exe:
        # also add the RPATH search path to the executable
        os.system("%s -add_rpath \"@executable_path/../lib\" %s >> /dev/null 2>&1" % \
                                                       (install_name_tool,file))
        os.system("%s -add_rpath \"@executable_path/../bundled/lib\" %s >> /dev/null 2>&1" % \
                                                       (install_name_tool,file))

    for line in lines:
        line = line.rstrip()

        if (line.find(".dylib") != -1 or line.find(".so") != -1) and line.find("@rpath") == -1:
            words = line.split()

            if len(words) > 1:
                oldpath = words[0]

                if oldpath.startswith(sire_root) or oldpath.find("libz.") != -1:
                    updateRPATH(file, oldpath, exe)
                else:
                    if not oldpath in external_libraries:
                        external_libraries[oldpath] = []

                    external_libraries[oldpath].append(file)


def scanDir(dir):
    """Scan this directory for all libraries and executables that
       link to shared libraries and that may have to be adjusted to
       use @rpath rather than an absolute file path"""
    print("\n\nScanning %s" % dir, end="", flush=True)

    files = os.listdir(dir)

    for file in files:
        if file.find(".dylib") != -1 or file.find(".so") != -1:
            scanLibrary("%s/%s" % (dir,file))
        elif os.path.islink("%s/%s" % (dir,file)):
            pass
        elif (not file.startswith(".")) and os.path.isdir("%s/%s" % (dir,file)):
            if file.find("__pycache__") == -1:
                scanDir("%s/%s" % (dir,file))
        elif file.find(".py") == -1 and file.find(".txt") == -1 and \
             file.find(".decTest") == -1:
            line = os.popen("file %s/%s" % (dir,file)).readline()

            if line.find("executable") != -1:
                scanLibrary("%s/%s" % (dir,file), True)

scanDir(sire_libdir)
scanDir(sire_bindir)
scanDir(sire_bundle_libdir)
scanDir(sire_bundle_bindir)

libs = list( external_libraries.keys() )
libs.sort()

print("\n**** External dependent libraries are ****")
for lib in libs:
    if lib.find("libSystem.") == -1 and lib.find("libc++.") == -1 and \
       lib.find("libstdc++.") == -1 and lib.startswith("/"):
        print("\nEXTERNAL DEPENDENCY = %s\nDEPENDENT OBJECTS = %s" % (lib, external_libraries[lib]))

