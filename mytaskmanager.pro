HEADERS       = window.h
SOURCES       = main.cpp \
                window.cpp
RESOURCES     = mytaskmanager.qrc

QT += widgets
CONFIG += c++14

# install
target.path = $$C:/MyTaskManager
INSTALLS += target
