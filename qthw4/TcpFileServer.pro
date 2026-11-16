QT += widgets network
TEMPLATE = app
TARGET = TcpFileServer
CONFIG += c++17
SOURCES += main.cpp \
           tcpfileserver.cpp
HEADERS += tcpfileserver.h
