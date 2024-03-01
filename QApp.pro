QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 no_keywords warn_on

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32 {
  win32-msvc*{
    QMAKE_CXXFLAGS_WARN_ON ~= s/-W./-W4
    QMAKE_CXXFLAGS += /MP /permissive-

    LIBS += -luser32
  }

  win32-g++*{

  }

  win32-clang*{

  }
}

linux {
  linux-g++*{
  }

  linux-clang*{
  }
}

macx {
  macx-g++*{
  }

 macx-clang*{
 }
}
#CONFIG += conan_basic_setup
include($${OUT_PWD}/conanbuildinfo.pri)

# Qwt
DEFINES += $$CONAN_DEFINES_QWT
INCLUDEPATH += $$CONAN_INCLUDEPATH_QWT
LIBS += $$CONAN_LIBDIRS_QWT $$CONAN_LIBS_QWT

#OneApi TBB
DEFINES += $$CONAN_DEFINES_ONETBB
INCLUDEPATH += $$CONAN_INCLUDEPATH_ONETBB
LIBS += $$CONAN_LIBDIRS_ONETBB $$CONAN_LIBS_ONETBB
# this path contains required dlls and is added to the run environment by QtCreator
# make sure to check Run option "Add build library search path to Path"
LIBS += -L$$CONAN_BINDIRS_ONETBB


SOURCES += \
  Except.cpp \
    Interface/MainWindow.cpp \
    Application.cpp \
    ApplicationImpl.cpp \
    main.cpp \


HEADERS += \
  Except.h \
    Interface/MainWindow.h \
    Application.h \
    ApplicationImpl.h \


FORMS += \
    Interface/MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
