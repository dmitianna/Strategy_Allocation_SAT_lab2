QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        Allocator/Allocator.cpp \
        SAT_DPLL/BBV.cpp \
        SAT_DPLL/boolequation.cpp \
        SAT_DPLL/boolinterval.cpp

HEADERS += \
    Allocator/Allocator.h \
    Allocator/DataTypes.h \
    SAT_DPLL/IBranchingStrategy.h \
    SAT_DPLL/BBV.h \
    SAT_DPLL/NodeBoolTree.h \
    SAT_DPLL/boolequation.h \
    SAT_DPLL/boolinterval.h



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/Allocator
INCLUDEPATH += $$PWD/SAT_DPLL
