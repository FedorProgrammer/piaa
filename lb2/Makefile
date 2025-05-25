CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -pedantic
EXT=cpp

SRC_DIR=src
OBJ_DIR=obj
JSON_DIR=json

SRC_FILES=$(wildcard $(SRC_DIR)/*.$(EXT))
OBJ_FILES=$(patsubst $(SRC_DIR)/%.$(EXT),$(OBJ_DIR)/%.o,$(SRC_FILES))

EXEC=$(OBJ_DIR)/exec

INCLUDES=-Iinclude

$(shell mkdir -p $(OBJ_DIR) $(JSON_DIR))

all: $(EXEC)

$(EXEC) : $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.$(EXT)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: $(EXEC)
	./$(EXEC)

valgrind: $(EXEC)
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./$(EXEC)

clean:
	rm -rf $(OBJ_DIR) $(JSON_DIR)

.PHONY: all run valgrind clean