# Makefile for my C++ re-write of Crafting Interpreters 
#
# Stefan Wong 2018


CXX=g++
CXXFLAGS = -Wall -Werror -g2 -O0 -std=c++11 
LDFLAGS := $(shell root-config --ldflags)


SRC_DIR=src
OBJ_DIR=obj

# Files 
INCLUDES := -I./$(SRC_DIR)
HEADERS := $(wildcard $(SRC_DIR)/*.hpp)
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)


$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< $(INCLUDES) -o $@ $(LIBS)

obj: $(OBJECTS)

all: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(INCLUDES) $(OBJECTS) -o intro $(LIBS)


print-%:
	@echo $* $($*)
