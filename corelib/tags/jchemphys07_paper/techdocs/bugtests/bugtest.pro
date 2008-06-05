SOURCES += test-regexp.cpp

QT += NETWORK
QT -= GUI
LIBS += -lpython2.3 -lboost_python
TARGET = test-regexp
TEMPLATE = app
!include($(ROOT_SRC_DIR)/build/local.pri): error(Cannot find configuration file $(ROOT_SRC_DIR)/build/local.pri! Run 'python make.py' first!)

