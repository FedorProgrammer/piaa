CCX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

EXEC = $(OBJ_DIR)/kmp

$(shell mkdir -p $(OBJ_DIR))

all: $(EXEC)

$(EXEC) : $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -o $@ $^

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(EXEC)

find: $(EXEC)
	./$(EXEC) find

cycle: $(EXEC)
	./$(EXEC) cycle

step: $(EXEC)
	./$(EXEC) step

.PHONY: all clean find cycle step