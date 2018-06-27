CONFIG += precompile_header
PRECOMPILED_HEADER = pch.hpp

HEADERS       = pch.hpp   \
                window.hpp \
                runguard.hpp \
                config.hpp \
    xclientpicker.hpp \
    profile_selector.hpp

SOURCES       = main.cpp \
                window.cpp \
                runguard.cpp \
                config.cpp \
    xclientpicker.cpp \
    profile_selector.cpp

RESOURCES     = dvc_toggler_linux.qrc

QT           += xml svg core gui widgets

release: DESTDIR = build/release

QMAKE_LFLAGS += -lX11 -lXext -ldl



