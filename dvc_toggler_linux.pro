CONFIG += precompile_header c++17
QMAKE_CXXFLAGS += -std=c++17

PRECOMPILED_HEADER = pch.hpp

HEADERS       = pch.hpp   \
                window.hpp \
                runguard.hpp \
                config.hpp \
    xclientpicker.hpp \
    profile_selector.hpp \
    procwatch.hpp

SOURCES       = main.cpp \
                window.cpp \
                config.cpp \
    profile_selector.cpp \
    proc_watch.cpp \
    run_guard.cpp \
    x_client_picker.cpp

RESOURCES     = dvc_toggler_linux.qrc

QT           += xml svg core gui widgets

release: DESTDIR = build/release

QMAKE_LFLAGS += -lX11 -lXext -ldl



