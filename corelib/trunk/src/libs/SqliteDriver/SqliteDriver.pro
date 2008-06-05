
TARGET	 = SqliteDriver

TEMPLATE = lib
sire.root = ../../.. # AUTO_GENERATED_LINE
sire.build = $$sire.root/build # AUTO_GENERATED_LINE
sire.config = $$sire.root/configure.pri # AUTO_GENERATED_LINE
sire.global = $$sire.build/global.pri # AUTO_GENERATED_LINE
!include($$sire.global): error(Cannot find global configure file!) # AUTO_GENERATED_LINE

DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

HEADERS		= qsql_sqlite.h \
              btree.h \
              hash.h \
              keywordhash.h \
              opcodes.h \
              os.h \
              os_common.h \
              pager.h \
              parse.h \
              sqlite3.h \
              sqliteInt.h \
              vdbe.h \
              vdbeInt.h
              
SOURCES		= qsql_sqlite.cpp \
              alter.c \
              analyze.c \
              attach.c \
              auth.c \
              btree.c \
              build.c \
              callback.c \
 	          date.c \
              delete.c \
              expr.c \
              func.c \
              hash.c \
              insert.c \
              legacy.c \
              main.c \
              opcodes.c \
              pager.c \
              parse.c \
              pragma.c \
              prepare.c \
              printf.c \
              random.c \
              select.c \
              table.c \
              tokenize.c \
              trigger.c \
              update.c \
              utf.c \
              util.c \
              vacuum.c \
              vdbeapi.c \
	          vdbeaux.c \
              vdbe.c \
              vdbefifo.c \
              vdbemem.c \
              where.c

unix {
    SOURCES += os_unix.c
    HEADERS += os_unix.h
}

win32 {
    SOURCES += os_win.c
    HEADERS += os_win.h
}
