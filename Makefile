CFLAGS=-O2 -fopenmp -std=c++20 `Magick++-config --cxxflags --cppflags`
LDFLAGS=`Magick++-config --ldflags --libs` -lSDL2

SOURCES=src/*.cpp optim/*.cpp main.cpp
HEADERS=include/*.h optim/*.h

nbody: $(SOURCES) $(HEADERS)
	g++ $(SOURCES) -o nbody $(CFLAGS) $(LDFLAGS)

nbodyGPU:
	nvcc ./src/nbodyGPU.cu -o nbodyGPU -arch=sm_60 -std=c++20 -I/usr/local/cuda/include --expt-relaxed-constexpr

clean:
	rm -f *.o
	rm -f ./nbody
	rm -f ./nbodyGPU
