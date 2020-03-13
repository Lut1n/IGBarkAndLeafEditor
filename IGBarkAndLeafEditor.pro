QT += quick
QT += opengl

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DataControl/groundgenerator.cpp \
    DataControl/loadingtask.cpp \
    DataControl/rockgenerator.cpp \
    DataControl/treegenerator.cpp \
    DataModel/grounddescriptor.cpp \
    DataModel/rockdescriptor.cpp \
    DataModel/treedescriptor.cpp \
    main.cpp \
    GuiContext//barkmodel.cpp \
    GuiContext/impgearsconfig.cpp \
    GuiContext/leafmodel.cpp \
    GuiContext/treemodel.cpp \
    GuiContext/loadinginfo.cpp \
    GuiContext/displayoptions.cpp \
    CustomView/impgearsitem.cpp \
    CustomView/impgearspainter.cpp \
    DataModel/barkdescription.cpp \
    DataModel/leafdescription.cpp \
    DataModel/geometrydescription.cpp \
    DataControl/texturecache.cpp \
    DataControl/loadingtaskqueue.cpp \
    DataControl/texturegenerator.cpp \
    ProceduralScene/leafrenderer.cpp \
    ProceduralScene/mainRenderer.cpp \
    ProceduralScene/proceduralskybox.cpp \
    ProceduralScene/barkrenderer.cpp \
    ProceduralScene/treeandislescene.cpp

# use function file
SOURCES += $$files("ImpGears/*.cpp", true)


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    DataControl/groundgenerator.h \
    DataControl/loadingtask.h \
    DataControl/rockgenerator.h \
    DataControl/treegenerator.h \
    DataModel/grounddescriptor.h \
    DataModel/rockdescriptor.h \
    DataModel/treedescriptor.h \
    GuiContext/barkmodel.h \
    GuiContext/impgearsconfig.h \
    GuiContext/leafmodel.h \
    GuiContext/treemodel.h \
    GuiContext/loadinginfo.h \
    GuiContext/displayoptions.h \
    CustomView/impgearsitem.h \
    CustomView/impgearspainter.h \
    DataModel/barkdescription.h \
    DataModel/leafdescription.h \
    DataModel/geometrydescription.h \
    DataControl/texturecache.h \
    DataControl/loadingtaskqueue.h \
    DataControl/texturegenerator.h \
    ProceduralScene/barkrenderer.h \
    ProceduralScene/leafrenderer.h \
    ProceduralScene/mainRenderer.h \
    ProceduralScene/proceduralskybox.h \
    ProceduralScene/treeandislescene.h

HEADERS += $$files("ImpGears/*.h", true)

INCLUDEPATH += ImpGears/

DEFINES += IMPGEARS_STATIC

DISTFILES +=
