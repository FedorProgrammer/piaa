CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

SRC_DIR = src
INCLUDE_DIR = include
BENCH_DIR = benchmarks
OBJ_DIR = obj
VISUAL_DIR = visual

COMMON_SRC = $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))

COMMON_OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(COMMON_SRC:.cpp=.o)))
MAIN_OBJ = $(OBJ_DIR)/main.o $(COMMON_OBJ)
BENCH_OBJ = $(OBJ_DIR)/benchmarks.o $(COMMON_OBJ)

MAIN_EXEC = $(OBJ_DIR)/tile
BENCH_EXEC = $(OBJ_DIR)/bench_squares

INCLUDES = -I.

$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(VISUAL_DIR)/output)

all: $(MAIN_EXEC) $(BENCH_EXEC)

$(MAIN_EXEC): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(BENCH_EXEC): $(BENCH_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(OBJ_DIR)/benchmarks.o: $(BENCH_DIR)/benchmarks.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

clean_visuals:
	rm -rf $(VISUAL_DIR)/output

deep_clean: clean clean_visuals

run: $(MAIN_EXEC)
	./$(MAIN_EXEC)

benchmark: $(BENCH_EXEC)
	./$(BENCH_EXEC)

visualize: 
	python3 $(VISUAL_DIR)/visual.py results.json

.PHONY: all clean clean_visuals deep_clean run benchmark visualize