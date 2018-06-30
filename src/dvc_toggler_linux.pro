CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17 -Wall -Wextra
QMAKE_LFLAGS += -lX11 -lXext -ldl -lXmu

TARGET = dvc-toggler

HEADERS = \
    window.hpp \
    runguard.hpp \
    config.hpp \
    xclientpicker.hpp \
    profile_selector.hpp \
    procwatch.hpp \
    nvidia/display.hpp \
    nvidia/nvidia.hpp \
    nvidia/libXNVCtrl/NVCtrl.h \

SOURCES = \
    main.cpp \
    window.cpp \
    config.cpp \
    profile_selector.cpp \
    proc_watch.cpp \
    run_guard.cpp \
    x_client_picker.cpp \
    nvidia/display.cpp \
    nvidia/nvidia.cpp \
    nvidia/libXNVCtrl/NVCtrl.c

RESOURCES     = dvc_toggler_linux.qrc

QT           += xml svg core gui widgets




