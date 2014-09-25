TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += /usr/local/libmemcached/include \
               /usr/local/libmemcached/include/libmemcached




LIBS += /usr/local/libmemcached/lib/libmemcached.so
#LIBS += /usr/local/libmemcached/lib/libmemcached.so \
        #/usr/local/libmemcached/lib/libhashkit.so \
        #/usr/local/libmemcached/lib/libmemcachedutil.so

SOURCES += \
    headers/encaplibmemcache.cpp \
    unitest.cpp \
    sources/util.cpp

HEADERS += \
    headers/encaplibmemcache.h \
    headers/util.h

