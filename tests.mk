# General settings
SRC_DIR = tests
BUILD_DIR = build/$(SRC_DIR)

CXX = g++
CXXFLAGS = -std=c++11 -Wall -MMD

LDLIBS = $(addprefix -l, $(ld))
LSLIBS = $(l) $(ls)

INCLUDES = $(addprefix -I, $(i))

VPATH = src: $(SRC_DIR)
vpath %.cpp $(SRC_DIR)

# *********************************** RULES ************************************

# Generic object builder
build/tests/%.cpp.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Generic runtime builder
build/tests/%: build/tests/%.cpp.o $(l)
	$(CXX) $(CXXFLAGS) -rdynamic $< $(LSLIBS) -o $@ $(LDFLAGS) $(LDLIBS)

# Custom builders put here
