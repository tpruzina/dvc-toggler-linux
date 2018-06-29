NVIDIA_DIR=./libXNVCtrl
NVIDIA_INCLUDES=-I$(NVIDIA_DIR) 
NVIDIA_STATIC=$(NVIDIA_DIR)/libXNVCtrl.a
CXX=clang++
CXXFLAGS=-std=c++03

LDFLAGS=-lX11 -lXext

all: test

test: nv
	$(CXX)  display.cpp nvidia.cpp $(NVIDIA_INCLUDES) $(LDFLAGS) $(CXXFLAGS) $(NVIDIA_STATIC) -D_DEBUG -o test_nvidia

nv: 
	make -C $(NVIDIA_DIR)

clean:
	make -C $(NVIDIA_DIR) clean
	rm -f test_nvidia
