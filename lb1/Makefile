CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

MATPLOT_FLAGS = -lmatplot
VISUAL_FLAGS = $(MATPLOT_FLAGS)

SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BENCH_DIR = benchmark
VISUAL_DIR = visual-output

INCLUDES = -I$(INCLUDE_DIR)

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(addprefix $(OBJ_DIR)/,$(notdir $(SRC_FILES:.cpp=.o)))
BENCH_FILE = benchmark.json

EXEC = $(OBJ_DIR)/tile

$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(BENCH_DIR))
$(shell mkdir -p $(VISUAL_DIR))

all: $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(VISUAL_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(EXEC)

clean_visuals:
	rm -rf $(VISUAL_DIR)/*

clean_benchmarks:
	rm -rf $(BENCH_DIR)/*

deep_clean: clean clean_visuals clean_benchmarks

run: $(EXEC)
	./$(EXEC) solve

run_benchmark: $(EXEC)
	touch $(BENCH_DIR)/$(BENCH_FILE)
	./$(EXEC) benchmark

run_visualize: $(EXEC)
	touch $(BENCH_DIR)/$(BENCH_FILE)
	./$(EXEC) visualize

run_steps: $(EXEC)
	./$(EXEC) steps

.PHONY: all clean clean_visuals clean_benchmarks deep_clean run run_benchmark run_visualize run_steps