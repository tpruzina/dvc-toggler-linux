NVIDIA_DIR=./libXNVCtrl
NVIDIA_INCLUDES=-I$(NVIDIA_DIR) 
NVIDIA_STATIC=$(NVIDIA_DIR)/libXNVCtrl.a
CXX ?= clang++
CXXFLAGS=-std=c++17

LDFLAGS=-lX11 -lXmu -lXext

all: nvidia xdisplay

nvidia: nv
	$(CXX)  nvidia.cpp $(NVIDIA_INCLUDES) $(LDFLAGS) $(CXXFLAGS) $(NVIDIA_STATIC) -DDEBUG_NVIDIA -o nvidia_test

xdisplay: nv
	$(CXX)  display.cpp $(NVIDIA_INCLUDES) $(LDFLAGS) $(CXXFLAGS) $(NVIDIA_STATIC) -DDEBUG_XDISPLAY -o xdisplay_test

nv: 
	make -C $(NVIDIA_DIR)



clean:
	make -C $(NVIDIA_DIR) clean
	rm -f *_test
