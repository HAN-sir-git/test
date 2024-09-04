RESOURCES += images.qrc

QMAKE_CFLAGS = -std=c++1y

# For GCC or Clang
QMAKE_CXXFLAGS += -Wno-unused-variable

## 定义 OpenCV 的路径
#OPENCV_PATH = $$PWD/../install/x64/mingw/bin

## 将 OpenCV 路径添加到 PATH 环境变量
#win32 {
#    QMAKE_POST_LINK += $$quote(set PATH=$$OPENCV_PATH;%PATH%)
#}

include(common.pri)
INCLUDEPATH +=  $$PWD/install/include/

INCLUDEPATH +=  $$PWD/libdxfrw/src/


LIBS += -L $$PWD/lib/libopencv_*.a

LIBS += -L $$PWD/dxflib/libdxfrw.a





# For MSVC
#win32:QMAKE_CXXFLAGS += /wd4101

include(ToolFunc/ToolFunc.pri)

HEADERS += mainwindow.h view.h chip.h \
    CommonDataForm/common_type.h \
    CommonDataForm/point.h \
    CommonDataForm/entity.h \
    CommonDataForm/polyline.h \
    CommonDataForm/line.h \
    CommonDataForm/common_mathfunc.h \
    CPltFileDataAnalysis/pltfiledata.h \
    CommonDataForm/data.h \
    geometryAnalysis/cgeometryanalysis.h \
    CustomGraphicsItem/customgraphicslineitem.h \
    CustomGraphicsItem/customgraphicspathitem.h \
    CustomGraphicsItem/customgraphicspolygonitem.h \
    CustomGraphicsItem/customgraphicsheader.h \
    CustomGraphicsItem/customgraphicsitem.h \
    OpencvAnalysis/cscenetoimage.h \
    Command/movecommand.h \
    cgraphicssence.h \
    Command/deletecommand.h \
    CDxfFileDataAnalysis/cdxfwriter.h \
    CommonDataForm/filterdxfrw.h \
    CommonDataForm/block.h



SOURCES += main.cpp \
    CommonDataForm/point.cpp \
    CommonDataForm/entity.cpp \
    CommonDataForm/polyline.cpp \
    CommonDataForm/line.cpp \
    CommonDataForm/common_mathfunc.cpp \
    CPltFileDataAnalysis/pltfiledata.cpp \
    CommonDataForm/data.cpp \
    geometryAnalysis/cgeometryanalysis.cpp \
    CustomGraphicsItem/customgraphicslineitem.cpp \
    CustomGraphicsItem/customgraphicspathitem.cpp \
    CustomGraphicsItem/customgraphicspolygonitem.cpp \
    CustomGraphicsItem/customgraphicsitem.cpp \
    OpencvAnalysis/cscenetoimage.cpp \
    Command/movecommand.cpp \
    cgraphicssence.cpp \
    Command/deletecommand.cpp \
    CDxfFileDataAnalysis/cdxfwriter.cpp \
    CommonDataForm/filterdxfrw.cpp \
    CommonDataForm/block.cpp



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

DISTFILES += \
    image/lunkuo.png
