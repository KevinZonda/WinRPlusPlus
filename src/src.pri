INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/maindialog.cpp

HEADERS += \
    $$PWD/config.h \
    $$PWD/maindialog.h

FORMS += \
    $$PWD/maindialog.ui

include($$PWD/core/core.pri)
