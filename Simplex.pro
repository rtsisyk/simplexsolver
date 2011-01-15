# #####################################################################
# Automatically generated by qmake (2.01a) ?? ??? 9 14:08:01 2009
# #####################################################################
TEMPLATE = app
TARGET = SimplexSolver
DEPENDPATH += . \
    gui \
    gui/output \
    core
INCLUDEPATH += . \
    gui \
    gui/output \
    core
TRANSLATIONS = locale/ru_RU.ts

HEADERS += gui/mainwindow.h \
    core/simplexmethod.h \
    core/simplexmethodparams.h \
    gui/qsourcetabledelegate.h \
    gui/qsourcetablewidget.h \
    gui/output/outputqtdocument.h \
    gui/output/outputbase.h
FORMS += gui/mainwindow.ui
SOURCES += main.cpp \
    core/simplexmethod.cpp \
    core/simplexmethodparams.cpp \
    gui/mainwindow.cpp \
    gui/qsourcetabledelegate.cpp \
    gui/qsourcetablewidget.cpp \
    gui/output/outputqtdocument.cpp \
    gui/output/outputbase.cpp
OTHER_FILES += gui/icon/main.ico \
    gui/icon/win32.rc \
    gui/icon/256x256.png \
    gui/icon/128x128.png \
    gui/icon/96x96.png \
    gui/icon/48x48.png \
    gui/icon/32x32.png \
    gui/icon/24x24.png \
    gui/icon/16x16.png

RESOURCES += gui/main.qrc
RC_FILE = gui/win32.rc

updateqm.input = TRANSLATIONS
updateqm.output = locale/${QMAKE_FILE_BASE}.qm
updateqm.commands = lrelease ${QMAKE_FILE_IN} -qm locale/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm
