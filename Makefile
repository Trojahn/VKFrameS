CC = g++ -std=c++11
UTILSHPP = src/Utils.hpp
UTILSCPP = src/Utils.cpp
KFHPP = src/ShotSegmentation.hpp
KFCPP = src/ShotSegmentation.cpp
LIBS = `pkg-config --libs --cflags opencv`

all: VKFrameS

Utils.o: $(UTILSHPP) $(UTILSCPP)
	@$(CC) -o src/$@ -c $(UTILSCPP)
	
KeyframeSelection.o: $(KFHPP) $(KFCPP)
	@$(CC) -o src/$@ -c $(KFHPP)

VKFrameS: src/Utils.o src/KeyframeSelection.o src/main.cpp
	@$(CC) src/main.cpp src/Utils.o src/KeyframeSelection.o -o VKFrameS $(LIBS)

clean:
	@rm -f VKFrameS src/*.o
