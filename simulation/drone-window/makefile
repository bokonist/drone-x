CXXFLAGS = -std=c++11
LIBS = -lGL -lglut -lGLU -lSOIL

drone: main.cpp drone.cpp
	$(CXX) $(CXXFLAGS) main.cpp drone.cpp -I. -o $@ $(LIBS)