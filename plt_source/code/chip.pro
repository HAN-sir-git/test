RESOURCES += images.qrc

QMAKE_CFLAGS = -std=c++1y

# For GCC or Clang
QMAKE_CXXFLAGS += -Wno-unused-variable

# For MSVC
#win32:QMAKE_CXXFLAGS += /wd4101


HEADERS += mainwindow.h view.h chip.h \
    CommonDataForm/common_type.h \
    CommonDataForm/point.h \
    CommonDataForm/entity.h \
    CommonDataForm/polyline.h \
    CommonDataForm/line.h \
    CommonDataForm/common_mathfunc.h \
    CPltFileDataAnalysis/pltfiledata.h \
    CommonDataForm/data.h


SOURCES += main.cpp \
    CommonDataForm/point.cpp \
    CommonDataForm/entity.cpp \
    CommonDataForm/polyline.cpp \
    CommonDataForm/line.cpp \
    CommonDataForm/common_mathfunc.cpp \
    CPltFileDataAnalysis/pltfiledata.cpp \
    CommonDataForm/data.cpp


SOURCES += mainwindow.cpp view.cpp chip.cpp

QT += widgets
qtHaveModule(printsupport): QT += printsupport
qtHaveModule(opengl): QT += opengl

build_all:!build_pass {
    CONFIG -= build_all
    CONFIG += release
}

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/chip
INSTALLS += target

