CXXFLAGS = -std=c++11
LIBS = -lGL -lglut -lGLU -lSOIL
drone: main.cpp movement.cpp draw.cpp
	$(CXX) $(CXXFLAGS) main.cpp movement.cpp draw.cpp -I. -o $@ $(LIBS)