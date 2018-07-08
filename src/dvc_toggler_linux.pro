CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17 -Wall -Wextra -g -O2 -I/usr/include/dbus-1.0 -I/usr/lib64/dbus-1.0/include
QMAKE_LIBS += -lX11 -lXext -ldl -lXmu -ldbus-1

TARGET = dvc-toggler

HEADERS = \
    config.hpp \
    nvidia/libXNVCtrl/NVCtrl.h \
    nvidia/nvidia.hpp \
    profile_selector.hpp \
    runguard.hpp \
    proc_watch.hpp \
    main_window.hpp \
    dbus/dbus_watch.hpp \
    nvidia/xdisplay.hpp


SOURCES = \
    config.cpp \
    main.cpp \
    nvidia/libXNVCtrl/NVCtrl.c \
    nvidia/nvidia.cpp \
    proc_watch.cpp \
    profile_selector.cpp \
    run_guard.cpp \
    main_window.cpp \
    dbus/dbus_watch.cpp \
    nvidia/xdisplay.cpp


RESOURCES     = dvc_toggler_linux.qrc

QT           += xml svg core gui widgets




