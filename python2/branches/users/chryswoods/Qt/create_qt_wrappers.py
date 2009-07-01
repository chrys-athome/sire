
import os
import sys

from glob import glob

from pyplusplus.module_builder import module_builder_t
from pyplusplus.module_builder.call_policies import return_internal_reference

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("../AutoGenerate")
from create_wrappers import *

wrap_classes = [ "QBool",
                 "QByteArray",
                 "QChar",
                 "QDataStream",
                 "QDate",
                 "QDateTime",
                 "QDir",
                 "QIODevice",
                 "QString",
                 "QTime",
                 "QUuid",

                 #"QFlags<QDir::SortFlag>",
                 #"QFlags<QDir::Filter>",
                 #"QFlags<QFile::Permission>",
                 "QFlags<QIODevice::OpenModeFlag>"
                 #"QFlags<QTextStream::NumberFlag>"
               ]

huge_classes = []

aliases = { "QFlags<QDir::SortFlag" : "SortFlags",
            "QFlags<QDir::Filter" : "Filters",
            "QFlags<QFile::Permission" : "Permissions",
            "QFlags<QIODevice::OpenModeFlag" : "OpenMode",
            "QFlags<QTextStream::NumberFlag" : "NumberFlags" 
          }

def fix_QByteArray(c):
    c.decls( "fill" ).call_policies = call_policies.return_self()
    c.decls( "prepend" ).call_policies = call_policies.return_self()
    c.decls( "append" ).call_policies = call_policies.return_self()
    c.decls( "insert" ).call_policies = call_policies.return_self()
    c.decls( "remove" ).call_policies = call_policies.return_self()
    c.decls( "replace" ).call_policies = call_policies.return_self()
    c.decls( "setNum" ).call_policies = call_policies.return_self()

def fix_QString(c):
    c.decls( "fill" ).call_policies = call_policies.return_self()
    c.decls( "insert" ).call_policies = call_policies.return_self()
    c.decls( "prepend" ).call_policies = call_policies.return_self()
    c.decls( "append" ).call_policies = call_policies.return_self()
    c.decls( "remove" ).call_policies = call_policies.return_self()
    c.decls( "replace" ).call_policies = call_policies.return_self()
    c.decls( "setUnicode" ).call_policies = call_policies.return_self()
    c.decls( "setNum" ).call_policies = call_policies.return_self()

special_code = { "QByteArray" : fix_QByteArray, 
                 "QString" : fix_QString }

aliases = { "QFlags<QDir::SortFlag>" : "SortFlags",
            "QFlags<QDir::Filter>" : "Filters",
            "QFlags<QFile::Permission>" : "Permissions",
            "QFlags<QIODevice::OpenModeFlag>" : "OpenMode",
            "QFlags<QTextStream::NumberFlag>" : "NumberFlags" 
          }

implicitly_convertible = [ ("QDir::SortFlag", "QFlags<QDir::SortFlag>"),
                           ("QDir::Filter", "QFlags<QDir::Filter>"),
                           ("QFile::Permission", "QFlags<QFile::Permission>"),
                           ("QIODevice::OpenModeFlag", "QFlags<QIODevice::OpenModeFlag>"),
                           ("QTextStream::NumberFlag", "QFlags<QTextStream::NumberFlag>"),
                           ("QString", "QFileInfo"),
                           ("QString", "QFile"),
                           ("QString", "QDir")
                         ]

#give the namespace and header files for the module to export
modulename = "Qt"

headerfiles = glob("qtheaders/*.h")

FILE = open("sireqt_headers.h", "w")

for headerfile in headerfiles:
    print >>FILE,"#include \"%s\"" % headerfile

FILE.close()

qtdir = os.getenv("QTDIR")

if (qtdir is None):
    print "You must set the environmental variable QTDIR to the location " + \
          "of the Qt4 header files"
    sys.exit(-1)

qt_include_dirs = [ qtdir, "%s/QtCore" % qtdir ]

#construct a module builder that will build the module's wrappers
mb = module_builder_t( files = ["sireqt_headers.h"],
                       include_paths = qt_include_dirs,
                       define_symbols = ["GCCXML_PARSE",
                                         "SIRE_SKIP_INLINE_FUNCTIONS",
                                         "SIRE_INSTANTIATE_TEMPLATES"] )

#get rid of all virtual python functions - this is to stop slow wrapper code
#from being generated for C++ virtual objects
for calldef in mb.calldefs():
    try:
        calldef.virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL
    except:
        pass

#add calls to register hand-written wrappers
mb.add_declaration_code( "#include \"sireqt_containers.h\"" )
mb.add_registration_code( "register_SireQt_containers();", tail=False )

include_files = [ "<QString>", "<QByteArray>", "<QFile>", "<QFileInfo>", 
                  "<QDir>", "<QTextStream>", "<QDateTime>", "<QLocale>",
                  "<QUuid>", "<qnamespace.h>" ]

mb.enums().include()

#export each class in turn
for classname in wrap_classes:
   #tell the program to write wrappers for this class
   export_class(mb, classname, aliases, [], special_code)

register_implicit_conversions(mb, implicitly_convertible)

write_wrappers(mb, modulename, huge_classes, include_files)

#now write a CMakeFile that contains all of the autogenerated files
FILE = open("CMakeAutogenFile.txt", "w")

print >>FILE,"# WARNING - AUTOGENERATED FILE - CONTENTS WILL BE OVERWRITTEN!"
print >>FILE,"set ( PYPP_SOURCES"

pyppfiles = glob("*.pypp.cpp")

for pyppfile in pyppfiles:
    print >>FILE,"       %s" % pyppfile

print >>FILE,"       sireqt_containers.cpp"

print >>FILE,"    )"

FILE.close()

