CFLAGS=-O2 -std=c++20 -lpthread
LDFLAGS=`Magick++-config --cxxflags --cppflags` `Magick++-config --ldflags --libs` -lSDL2

SOURCES=src/*.cpp optim/*.cpp
HEADERS=include/*.h optim/*.h

nbody: $(SOURCES) $(HEADERS)
	g++ $(SOURCES) main.cpp -o nbody $(CFLAGS) $(LDFLAGS)

nbodyGPU:
	nvcc ./src/nbodyGPU.cu -o nbodyGPU -arch=sm_60 -std=c++20 -I/usr/local/cuda/include --expt-relaxed-constexpr

benchmark: $(SOURCES) $(HEADERS) benchmark.cpp
	g++ $(SOURCES) benchmark.cpp -o benchmark $(CFLAGS)

clean:
	rm -f *.o
	rm -f ./nbody
	rm -f ./nbodyGPU
