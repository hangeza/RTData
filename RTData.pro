QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/Array.cpp \
    src/importwizard_impl.cpp \
    src/main.cpp \
    src/multidialog.cpp \
    src/QHistogram.cpp \
    src/RenderArea.cpp \
    src/rtdata.cpp \

HEADERS += \
    src/Array.h \
    src/importwizard_impl.h \
    src/multidialog.h \
    src/QHistogram.h \
    src/QPoint3.h \
    src/RenderArea.h \
    src/rtdata.h \

FORMS += \
    src/rtdata.ui \
    src/importwizard.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
