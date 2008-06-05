
import os
import sys

from pyplusplus.module_builder import module_builder_t
from pyplusplus.module_builder.call_policies import return_internal_reference

from pygccxml.declarations.matchers import access_type_matcher_t
from pygccxml import declarations

sys.path.append("..")
from sireutils import *

wrap_classes = [ "QDir",
                 "QFile",
                 "QFileInfo",
                 "QByteArray",
                 "QChar",
                 "QDataStream",
                 "QIODevice",
                 "QTextStream",
                 "QDateTime",
                 "QDate",
                 "QTime",
                 "QVariant",
                 
                 "QFlags<QDir::SortFlag>",
                 "QFlags<QDir::Filter>",
                 "QFlags<QFile::Permission>",
                 "QFlags<QIODevice::OpenModeFlag>",
                 "QFlags<QTextStream::NumberFlag>"
               ]

huge_classes = []

aliases = { "QFlags<QDir::SortFlag" : "SortFlags",
            "QFlags<QDir::Filter" : "Filters",
            "QFlags<QFile::Permission" : "Permissions",
            "QFlags<QIODevice::OpenModeFlag" : "OpenMode",
            "QFlags<QTextStream::NumberFlag" : "NumberFlags" 
          }

extra_includes = []

def exposeFunctions(c, funcs):
    for func in funcs:
       c.decls(func).include()

def expose_QDir(c):

    #remove all declarations
    c.decls().exclude()
    
    #now add in specified functions...
    c.constructors().include()
    
    funcs = [ "absoluteFilePath",
              "absolutePath",
              "canonicalPath",
              "cd",
              "cdUp",
              "count",
              "dirName",
              "entryInfoList",
              "entryList",
              "exists",
              "filePath",
              "filter",
              "isAbsolute",
              "isReadable",
              "isRelative",
              "isRoot",
              "makeAbsolute",
              "mkdir",
              "mkpath",
              "nameFilters",
              "path",
              "refresh",
              "relativeFilePath",
              "remove",
              "rename",
              "rmdir",
              "rmpath",
              "setFilter",
              "setNameFilters",
              "setPath",
              "setSorting",
              "sorting",
              "operator!=",
              "operator==",
              "operator[]",
              "addResourceSearchPath",
              "cleanPath",
              "current",
              "currentPath",
              "drives",
              "home",
              "homePath",
              "isAbsolutePath",
              "isRelativePath",
              "match",
              "root",
              "rootPath",
              "separator",
              "setCurrent",
              "temp",
              "tempPath"
            ]

    exposeFunctions(c,funcs)
    
    #include the public types
    c.decl("Filter").include()
    c.decl("Filters").include()
    c.decl("SortFlag").include()
    c.decl("SortFlags").include()

def expose_QFileInfo(c):

    #remove all declarations
    c.decls().exclude()
    
    #now add in specified functions...
    c.constructors().include()
    
    funcs = [ "absoluteDir",
              "absoluteFilePath",
              "absolutePath",
              "baseName",
              "caching",
              "canonicalFilePath",
              "canonicalPath",
              "completeBaseName",
              "completeSuffix",
              "created",
              "dir",
              "exists",
              "fileName",
              "filePath",
              "group",
              "groupId",
              "isAbsolute",
              "isDir",
              "isExecutable",
              "isFile",
              "isHidden",
              "isReadable",
              "isRelative",
              "isRoot",
              "isSymLink",
              "isWritable",
              "lastModified",
              "lastRead",
              "makeAbsolute",
              "owner",
              "ownerId",
              "path",
              "permission",
              "permissions",
              "readLink",
              "refresh",
              "setCaching",
              "setFile",
              "size",
              "suffix",
              "operator!=",
              "operator=="
            ]

    exposeFunctions(c,funcs)

def expose_QFile(c):

    #remove all declarations
    c.decls().exclude()
    
    #now add in specified functions...
    c.constructors().include()
    
    funcs = [ "atEnd",
              "copy",
              "error",
              "exists",
              "fileName",
              "flush",
              "handle",
              "isSequential",
              "link",
              "open",
              "permissions",
              "remove",
              "rename",
              "resize",
              "setFileName",
              "setPermissions",
              "size",
              "unsetError",
              "copy",
              "decodeName",
              "encodeName",
              "exists",
              "link",
              "permissions",
              "readLink",
              "setDecodingFunction",
              "setEncodingFunction",
              "setPermissions"
            ]

    exposeFunctions(c,funcs)
    
    c.decl("FileError").include()
    c.decl("Permission").include()

def expose_QByteArray(c):

    #remove all declarations
    c.decls().exclude()
    
    #now add in specified functions...
    c.constructors().include()
    
    funcs = [ "at",
              "capacity",
              "chop",
              "clear",
              "constData",
              "contains",
              "count",
              "endsWith",
              "indexOf",
              "isEmpty",
              "isNull",
              "lastIndexOf",
              "left",
              "leftJustified",
              "length",
              "mid",
              "push_back",
              "push_front",
              "reserve",
              "resize",
              "right",
              "rightJustified",
              "simplified",
              "size",
              "split",
              "squeeze",
              "startsWith",
              "toBase64",
              "toDouble",
              "toFloat",
              "toInt",
              "toLong",
              "toLongLong",
              "toLower",
              "toShort",
              "toUInt",
              "toULong",
              "toULongLong",
              "toUShort",
              "toUpper",
              "trimmed",
              "truncate",
              "operator!=",
              "operator==",
              "operator+=",
              "operator<",
              "operator<=",
              "operator>",
              "operator>=",
              "operator[]",
              "fromBase64",
              "fromRawData",
              "number"
            ]

    exposeFunctions(c,funcs)

def expose_QChar(c):

    #remove all declarations
    c.decls().exclude()
    
    #now add in specified functions...
    c.constructors().include()
    
    funcs = [ "category",
              "cell",
              "combiningClass",
              "decomposition",
              "decompositionTag",
              "digitValue",
              "direction",
              "hasMirrored",
              "isDigit",
              "isLetter",
              "isLetterOrNumber",
              "isLower",
              "isMark",
              "isNull",
              "isNumber",
              "isPrint",
              "isPunct",
              "isSpace",
              "isSymbol",
              "isUpper",
              "mirroredChar",
              "row",
              "toAscii",
              "toLatin1",
              "toLower",
              "toUpper",
              "unicodeVersion",
              "fromAscii",
              "fromLatin1"
            ]

    exposeFunctions(c,funcs)
    
    c.decl("Category").include()
    c.decl("Decomposition").include()
    c.decl("Direction").include()
    c.decl("Joining").include()
    c.decl("SpecialCharacter").include()
    c.decl("UnicodeVersion").include()

def expose_QDataStream(c):

    #remove all declarations
    c.decls().exclude()
    
    #now add in specified functions...
    c.constructors().include()
    
    funcs = [ "atEnd",
              "byteOrder",
              "readRawData",
              "resetStatus",
              "setByteOrder",
              "setDevice",
              "setStatus",
              "setVersion",
              "skipRawData",
              "status",
              "unsetDevice",
              "version"
            ]

    exposeFunctions(c,funcs)
    
    c.decl("ByteOrder").include()
    c.decl("Status").include()
    c.decl("Version").include()
    
    #c.decls("operator<<").call_policies = return_internal_reference
    #c.decls("operator>>").call_policies = return_internal_reference

def expose_QIODevice(c):

    #remove all declarations
    c.decls().exclude()
    
    #now add in specified functions...
    c.constructors().include()
    
    funcs = [ "atEnd",
              "bytesAvailable",
              "bytesToWrite",
              "canReadLine",
              "close",
              "errorString",
              "getChar",
              "isOpen",
              "isReadable",
              "isSequential",
              "isTextModeEnabled",
              "isWritable",
              "open",
              "openMode",
              "peek",
              "pos",
              "putChar",
              "read",
              "readAll",
              "readLine",
              "reset",
              "seek",
              "setTextModeEnabled",
              "size",
              "ungetChar",
              "waitForBytesWritten",
              "waitForReadyRead",
              "write"
            ] 

    exposeFunctions(c,funcs)
    
    c.decl("OpenModeFlag").include()

def expose_QTextStream(c):

    #remove all declarations
    c.decls().exclude()
    
    #now add in specified functions...
    c.constructors().include()
    
    funcs = [ "atEnd",
              "autoDetectUnicode",
              "fieldAlignment",
              "fieldWidth",
              "flush",
              "generateByteOrderMark",
              "integerBase",
              "numberFlags",
              "padChar",
              "read",
              "readAll",
              "readLine",
              "realNumberNotation",
              "realNumberPrecision",
              "reset",
              "resetStatus",
              "seek",
              "setAutoDetectUnicode",
              "setCodec",
              "setDevice",
              "setFieldAlignment",
              "setFieldWidth",
              "setGenerateByteOrderMark",
              "setIntegerBase",
              "setNumberFlags",
              "setPadChar",
              "setRealNumberNotation",
              "setRealNumberPrecision",
              "setStatus",
              "setString",
              "skipWhiteSpace",
              "status"
            ]

    exposeFunctions(c,funcs)
    
    c.decl("FieldAlignment").include()
    c.decl("NumberFlag").include()
    c.decl("RealNumberNotation").include()
    c.decl("Status").include()

def expose_QDateTime(c):

    #remove all declarations
    c.decls().exclude()
    
    #now add in specified functions...
    c.constructors().include()
    
    funcs = [ "addDays",
              "addMSecs",
              "addMonths",
              "addSecs",
              "addYears",
              "date",
              "daysTo",
              "isNull",
              "isValid",
              "secsTo",
              "setDate",
              "setTime",
              "setTimeSpec",
              "setTime_t",
              "time",
              "timeSpec",
              "toLocalTime",
              "toString",
              "toTimeSpec",
              "toTime_t",
              "toUTC",
              "operator!=",
              "operator<",
              "operator<=",
              "operator==",
              "operator>",
              "operator>=",
              "currentDateTime",
              "fromString"
            ]

    exposeFunctions(c,funcs)

def expose_QDate(c):

    #remove all declarations
    c.decls().exclude()
    
    #now add in specified functions...
    c.constructors().include()
    
    funcs = [ "addDays",
              "addMonths",
              "addYears",
              "day",
              "dayOfWeek",
              "dayOfYear",
              "daysInMonth",
              "daysInYear",
              "daysTo",
              "isNull",
              "isValid",
              "month",
              "setYMD",
              "toJulianDay",
              "toString",
              "weekNumber",
              "year",
              "operator!=",
              "operator<",
              "operator<=",
              "operator==",
              "operator>",
              "operator>=",
              "currentDate",
              "fromJulianDay",
              "fromString",
              "isLeapYear",
              "isValid",
              "longDayName",
              "longMonthName",
              "shortDayName",
              "shortMonthName"
            ]

    exposeFunctions(c,funcs)

def expose_QTime(c):

    #remove all declarations
    c.decls().exclude()
    
    #now add in specified functions...
    c.constructors().include()
    
    funcs = [ "addMSecs",
              "addSecs",
              "elapsed",
              "hour",
              "isNull",
              "isValid",
              "minute",
              "msec",
              "msecsTo",
              "restart",
              "second",
              "secsTo",
              "setHMS",
              "start",
              "toString",
              "operator!=",
              "operator<",
              "operator<=",
              "operator==",
              "operator>",
              "operator>=",
              "currentTime",
              "fromString",
              "isValid"
            ]

    exposeFunctions(c,funcs)

def expose_QVariant(c):

    #remove all declarations
    c.decls().exclude()
    
    #now add in specified functions...
    c.constructors().include()
    
    funcs = [ "canConvert",
              "clear",
              "convert",
              "isNull",
              "isValid",
              "toBool",
              "toByteArray",
              "toChar",
              "toDate",
              "toDateTime",
              "toDouble",
              "toInt",
              "toLine",
              "toLineF",
              "toList",
              "toLocale",
              "toLongLong",
              "toMap",
              "toPoint",
              "toPointF",
              "toRect",
              "toRectF",
              "toRegExp",
              "toSize",
              "toSizeF",
              "toString",
              "toStringList",
              "toTime",
              "toUInt",
              "toULongLong",
              "toUrl",
              "type",
              "typeName",
              "userType",
              "operator!=",
              "operator==",
              "nameToType",
              "typeToName"
            ]

    exposeFunctions(c,funcs)

    c.decl("Type").include()

special_code = { "QDir" : expose_QDir,
                 "QFileInfo" : expose_QFileInfo,
                 "QFile" : expose_QFile,
                 "QByteArray" : expose_QByteArray,
                 "QChar" : expose_QChar,
                 "QDataStream" : expose_QDataStream,
                 "QIODevice" : expose_QIODevice,
                 "QTextStream" : expose_QTextStream,
                 "QDateTime" : expose_QDateTime,
                 "QDate" : expose_QDate,
                 "QTime" : expose_QTime,
                 "QVariant" : expose_QVariant
               }

aliases = { "QFlags<QDir::SortFlag" : "SortFlags",
            "QFlags<QDir::Filter" : "Filters",
            "QFlags<QFile::Permission" : "Permissions",
            "QFlags<QIODevice::OpenModeFlag" : "OpenMode",
            "QFlags<QTextStream::NumberFlag" : "NumberFlags" 
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

incpaths = sys.argv[1:]
incpaths.insert(0, "../../")

#give the namespace and header files for the module to export
modulename = "Qt"

headerfiles = ["sireqt_headers.h"]

#construct a module builder that will build the module's wrappers
mb = module_builder_t( files=headerfiles, 
                       include_paths=incpaths,
                       define_symbols=["SKIP_BROKEN_GCCXML_PARTS"] )

populateNamespaces(mb)

for calldef in mb.calldefs():
    try:
      calldef.virtuality = declarations.VIRTUALITY_TYPES.NOT_VIRTUAL
    except:
      pass

#add calls to register hand-written wrappers
mb.add_declaration_code( "#include \"sireqt_containers.h\"" )
mb.add_registration_code( "register_SireQt_containers();", tail=False )

mb.add_declaration_code( "#include \"QtGlobal.py.h\"" )
mb.add_registration_code( "export_QtGlobal();", tail=False )

mb.add_declaration_code( "#include \"QString.py.h\"" )
mb.add_registration_code( "export_QString();", tail=False )

mb.calldefs().create_with_signature = True

#export each class in turn
for classname in wrap_classes:
   #tell the program to write wrappers for this class
   export_class(mb, classname, aliases, special_code)

register_implicit_conversions(mb, implicitly_convertible)

write_wrappers(mb, modulename, extra_includes, huge_classes)
