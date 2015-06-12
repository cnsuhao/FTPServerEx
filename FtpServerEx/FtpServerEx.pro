TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    CommonFtp.cpp \
    CommonSocket.cpp \
    FtpServer.cpp \
    IniReader.cpp \
    Lock.cpp \
    Thread.cpp \
    ThreadPool.cpp \
    Utilities.cpp \
    UserInfo.cpp \
    ServeClient.cpp \
    CommonFile.cpp \
    VirtualPath.cpp \
    DataTransport.cpp

include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    FtpServerEx.pro.user \
    Setting.ini

HEADERS += \
    CommonFtp.h \
    CommonSocket.h \
    FtpServer.h \
    IniReader.h \
    Lock.h \
    Thread.h \
    ThreadPool.h \
    Utilities.h \
    FtpRespString.h \
    UserInfo.h \
    ServeClient.h \
    CommonFile.h \
    VirtualPath.h \
    DataTransport.h \
    NoCopyable.h

QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread
