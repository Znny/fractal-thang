# Test Makefile - Dedicated to building and running tests
CXX = g++
BUILD_DIR = build
TEST_OBJ_DIR = $(BUILD_DIR)/obj-test
SRC_DIR = ../fractal-core/src
INCLUDE_DIR = ../fractal-core/include
TEST_DIR = cpp
GOOGLETEST_DIR = ../external/googletest

# Create test object directory
$(shell mkdir -p $(TEST_OBJ_DIR))

# Source files
TEST_SRC = $(wildcard $(TEST_DIR)/*.cpp)
PROJECT_SRC = $(filter-out $(SRC_DIR)/embinding.cpp $(SRC_DIR)/main.cpp,$(wildcard $(SRC_DIR)/*.cpp))

# Object files
TEST_OBJ = $(patsubst $(TEST_DIR)/%.cpp,$(TEST_OBJ_DIR)/test_%.o,$(TEST_SRC))
PROJECT_OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(TEST_OBJ_DIR)/project_%.o,$(PROJECT_SRC))

# Dependency files
TEST_DEPS = $(patsubst $(TEST_DIR)/%.cpp,$(TEST_OBJ_DIR)/test_%.d,$(TEST_SRC))
PROJECT_DEPS = $(patsubst $(SRC_DIR)/%.cpp,$(TEST_OBJ_DIR)/project_%.d,$(PROJECT_SRC))

# Include dependency files
-include $(TEST_DEPS)
-include $(PROJECT_DEPS)

# Compilation flags
TEST_CFLAGS = -Wall -Wextra -O2 -I$(INCLUDE_DIR) -I$(GOOGLETEST_DIR)/googletest/include -MMD -MP
TEST_CXXFLAGS = $(TEST_CFLAGS) -std=c++17

# Test executable
TEST_TARGET = $(BUILD_DIR)/test_runner

# Default target
all: test

# Build test executable
test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ) $(PROJECT_OBJ)
	@echo "Linking test executable..."
	$(CXX) $(TEST_CXXFLAGS) -o $(TEST_TARGET) $^ -L$(GOOGLETEST_DIR)/build/lib -lgtest -lgtest_main -lpthread

# Compile test files
$(TEST_OBJ_DIR)/test_%.o: $(TEST_DIR)/%.cpp
	@echo "Compiling test $< to $@..."
	$(CXX) $(TEST_CXXFLAGS) -c $< -o $@

# Compile project source files for tests
$(TEST_OBJ_DIR)/project_%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling project source $< to $@..."
	$(CXX) $(TEST_CXXFLAGS) -c $< -o $@

# Run tests
run: test
	@echo "Running tests..."
	./$(TEST_TARGET)

# Clean test artifacts
clean:
	rm -f $(TEST_OBJ) $(PROJECT_OBJ) $(TEST_DEPS) $(PROJECT_DEPS) $(TEST_TARGET)
	rm -rf $(TEST_OBJ_DIR)
	rm -rf $(BUILD_DIR)

# Build Google Test if not already built
setup-gtest:
	@echo "Setting up Google Test..."
	cd $(GOOGLETEST_DIR) && mkdir -p build && cd build && cmake .. && make

# Preserve dependency files
.PRECIOUS: $(TEST_DEPS) $(PROJECT_DEPS)

.PHONY: all test run clean setup-gtest 