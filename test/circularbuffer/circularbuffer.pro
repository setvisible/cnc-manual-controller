#isEmpty(TEMPLATE):TEMPLATE=app
TARGET       = tst_circularbuffer
CONFIG      += testcase
QT           = core testlib
SOURCES     += tst_circularbuffer.cpp

# Include:
INCLUDEPATH += ../../include

# Dependancies:
HEADERS  += \
    $$PWD/../../src/core/commandbuffer.h \
    $$PWD/../../src/core/commandframe.h \
    $$PWD/../../src/core/commandstep.h \
    $$PWD/../../src/core/circularbuffer.h

SOURCES += \
    $$PWD/../../src/core/commandbuffer.cpp \
    $$PWD/../../src/core/commandframe.cpp \
    $$PWD/../../src/core/commandstep.cpp \
    $$PWD/../../src/core/circularbuffer.cpp

