QT       += core gui
QT       += serialport
QT       += gui
QT       += datavisualization


QMAKE_CXXFLAGS += -gstabs+
UI_DIR=./UI

RC_ICONS = logo.ico

DEFINES += QT_DLL QWT_DLL
#LIBS += -L"D:\Qt\Qt5.13.2\5.13.2\mingw73_32\lib" -lqwtd
LIBS += -L"D:\Qt\Qt5.13.2\5.13.2\mingw73_32\lib" -lqwt
INCLUDEPATH += D:\Qt\Qt5.13.2\5.13.2\mingw73_32\include\Qwt

DEFINES += QT_DLL QUC_DLL
#LIBS  += -L$$PWD/sdk/ -lqucd
LIBS  += -L$$PWD/sdk/ -lquc
INCLUDEPATH += D:\Qt\Qt5.13.2\5.13.2\mingw73_32\include\Quc


#INCLUDEPATH += $$PWD/sdk
#CONFIG(release, debug|release){
#LIBS        += -L$$PWD/sdk/ -lquc
#} else {
#unix {LIBS  += -L$$PWD/sdk/ -lquc}
#else {LIBS  += -L$$PWD/sdk/ -lqucd}
#}



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


# 一般加上pthread就可以
LIBS += -pthread
# 如果你的QT工程CONFIG中没有配置c++11，最好把下面两行也一并加上吧
QMAKE_CXXFLAGS += -pthread
QMAKE_CXXFLAGS += -std=c++11



CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calibration.cpp \
    flightstate.cpp \
    main.cpp \
    mainwindow.cpp \
    remotectrl.cpp \
    sendandreceive.cpp \
    serialportthread.cpp \
    serialsetup.cpp \
    setparameter.cpp \
    waveshow.cpp

HEADERS += \
    Headfile.h \
    calibration.h \
    flightstate.h \
    mainwindow.h \
    remotectrl.h \
    sendandreceive.h \
    serialportthread.h \
    serialsetup.h \
    setparameter.h \
    waveshow.h

FORMS += \
    calibration.ui \
    flightstate.ui \
    mainwindow.ui \
    remotectrl.ui \
    sendandreceive.ui \
    serialsetup.ui \
    setparameter.ui \
    waveshow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    image/blog.png \
    image/clearwave.png \
    image/defaultwave.png \
    image/help.png \
    image/stopwave.png

RESOURCES += \
    Resources.qrc
