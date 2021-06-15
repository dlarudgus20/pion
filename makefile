CXX_SRC := $(wildcard pion/*.cpp) pion/tdogl/Bitmap.cpp
CUDA_SRC := $(wildcard pion/*.cu)

TARGET = EXE

CXXFLAGS = -std=c++14 -O3
LDFLAGS = -lGLEW -lglfw -lGL -lX11

$(TARGET): $(CXX_SRC)
	nvcc $(CXXFLAGS) -o $(TARGET) $(CXX_SRC) $(CUDA_SRC) $(LDFLAGS)

clean:
	-rm -f *.o
	-rm -f $(TARGET)
