CPPFLAGS := -O2 -g -std=c++11 -pthread -Wl,--no-as-needed
LDLIBS := `pkg-config --libs --cflags opencv`
OBJS = Utils.o KeyframeSelection.o
PROG = VKFrameS
CXX = g++

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) $(CPPFLAGS) $(OBJS) src/main.cpp -o $@ $(LDLIBS)

Utils.o: src/Utils.cpp src/Utils.hpp
	$(CXX) $(CPPFLAGS) -c src/Utils.cpp -o $@
	
KeyframeSelection.o: src/KeyframeSelection.cpp src/KeyframeSelection.hpp
	$(CXX) $(CPPFLAGS) -c src/KeyframeSelection.cpp -o $@

clean:
	@rm -f $(PROG) *.o
