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


INCLUDEPATH +=  D:\WorkSpace\github_project\plt\test\plt_source\code\install\include\


LIBS += -L D:\WorkSpace\github_project\plt\test\plt_source\code\lib\libopencv_*.a



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
    CommonDataForm/data.h \
    geometryAnalysis/cgeometryanalysis.h \
    CustomGraphicsItem/customgraphicslineitem.h \
    CustomGraphicsItem/customgraphicspathitem.h \
    CustomGraphicsItem/customgraphicspolygonitem.h \
    CustomGraphicsItem/customgraphicsheader.h \
    CustomGraphicsItem/customgraphicsitem.h


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
    CustomGraphicsItem/customgraphicsitem.cpp


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

