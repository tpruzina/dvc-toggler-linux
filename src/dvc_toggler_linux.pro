CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17 -Wall -Wextra -g -O2
QMAKE_LIBS += -lX11 -lXext -ldl -lXmu

TARGET = dvc-toggler

HEADERS = \
    config.hpp \
    nvidia/display.hpp \
    nvidia/libXNVCtrl/NVCtrl.h \
    nvidia/nvidia.hpp \
    profile_selector.hpp \
    runguard.hpp \
    proc_watch.hpp \
    main_window.hpp


SOURCES = \
    config.cpp \
    main.cpp \
    nvidia/display.cpp \
    nvidia/libXNVCtrl/NVCtrl.c \
    nvidia/nvidia.cpp \
    proc_watch.cpp \
    profile_selector.cpp \
    run_guard.cpp \
    main_window.cpp


RESOURCES     = dvc_toggler_linux.qrc

QT           += xml svg core gui widgets




