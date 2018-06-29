CONFIG += precompile_header c++17
QMAKE_CXXFLAGS += -std=c++17 -Wall -Wextra

PRECOMPILED_HEADER = pch.hpp

LIBS += -Lnvidia/libXNVCtrl nvidia/libXNVCtrl/libXNVCtrl.a

HEADERS = \
    pch.hpp \
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
    nvidia/nvidia.cpp

RESOURCES     = dvc_toggler_linux.qrc

QT           += xml svg core gui widgets

release: DESTDIR = ../build

QMAKE_LFLAGS += -lX11 -lXext -ldl



