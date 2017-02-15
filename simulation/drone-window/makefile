CXXFLAGS = -std=c++11
LIBS = -lGL -lglut -lGLU -lSOIL
drone: main.cpp frameupdate.cpp movement.cpp draw.cpp
	$(CXX) $(CXXFLAGS) main.cpp frameupdate.cpp movement.cpp draw.cpp -I. -o $@ $(LIBS)