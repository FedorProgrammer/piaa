CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -pedantic
EXT=cpp

SRC_DIR=src
OBJ_DIR=obj

SRC_FILES=$(wildcard $(SRC_DIR)/*.$(EXT))
OBJ_FILES=$(patsubst $(SRC_DIR)/%.$(EXT),$(OBJ_DIR)/%.o,$(SRC_FILES))

EXEC=$(OBJ_DIR)/exec
INCLUDES=-Iinclude

$(shell mkdir -p $(OBJ_DIR))

all: $(EXEC)

$(EXEC) : $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.$(EXT)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: $(EXEC)
	./$(EXEC) run

step: $(EXEC)
	./$(EXEC) step

prescriptions-no-step: $(EXEC)
	./$(EXEC) prescriptions-no-step

prescriptions-with-step: $(EXEC)
	./$(EXEC) prescriptions-with-step

clean:
	rm -rf $(OBJ_DIR)/*.o $(EXEC)

.PHONY: all run step prescriptions-no-step prescriptions-with-step clean