CFLAGS=-O2 -fopenmp -std=c++17 `Magick++-config --cxxflags --cppflags`
LDFLAGS=`Magick++-config --ldflags --libs` -lSDL2

SOURCES=src/*.cpp main.cpp
HEADERS=include/*.h

nbody: $(SOURCES) $(HEADERS)
	g++ src/*.cpp main.cpp -o nbody $(CFLAGS) $(LDFLAGS)

nbodyGPU:
	nvcc ./src/nbodyGPU.cu -o nbodyGPU -arch=sm_60 -std=c++11 -I/usr/local/cuda/include --expt-relaxed-constexpr

clean:
	rm -f *.o
	rm -f ./nbody
	rm -f ./nbodyGPU