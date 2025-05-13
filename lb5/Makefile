CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -pedantic -g -O0
LDFLAGS=-lcgraph -lgvc
EXT=cpp

SRC_DIR=src
OBJ_DIR=obj
VISUAL_DIR=pic

SRC_FILES=$(wildcard $(SRC_DIR)/*.$(EXT))
OBJ_FILES=$(patsubst $(SRC_DIR)/%.$(EXT),$(OBJ_DIR)/%.o,$(SRC_FILES))

EXEC=$(OBJ_DIR)/exec
INCLUDES=-Iinclude

$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(VISUAL_DIR))

all: $(EXEC)

$(EXEC) : $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.$(EXT)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run-default: $(EXEC)
	./$(EXEC) run-default

run-wildcard: $(EXEC)
	./$(EXEC) run-wildcard

step-default: $(EXEC)
	./$(EXEC) step-default

step-wildcard: $(EXEC)
	./$(EXEC) step-wildcard

visualize: $(EXEC)
	./$(EXEC) visualize

valgrind: $(EXEC)
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./$(EXEC) run-default

clean:
	rm -rf $(OBJ_DIR)/*.o $(EXEC)

clean-visual:
	rm -rf $(VISUAL_DIR)/*

.PHONY: all run-default run-wildcard step-default step-wildcard valgrind clean clean-visual