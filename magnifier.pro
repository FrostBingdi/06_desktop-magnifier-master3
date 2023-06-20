QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++2a

QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS += -Werror

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    monitor.cpp \
    settingbar.cpp \
    viewfinder.cpp

HEADERS += \
    monitor.h \
    settingbar.h \
    viewfinder.h

FORMS += \
    monitor.ui \
    settingbar.ui \
    viewfinder.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    resources.qrc

RC_FILE = app.rc

LIBS += -lgdi32 -luser32

RC_ICONS = MagnifierIcon.ico
