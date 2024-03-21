# Compiler and compiler flags
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic -fpermissive
LDFLAGS :=

# Directories
SRC_DIR := src
OBJ_DIR := obj
INC_DIR := include
BIN_DIR := bin

# Source and header file lists
SRC := $(wildcard $(SRC_DIR)/*.cpp)
HEADERS := $(wildcard $(INC_DIR)/*.h)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

# Executable name
TARGET := $(BIN_DIR)/main

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

debug:
	$(CXX) ConsoleLoggerHelper.cpp -o $(BIN_DIR)/ConsoleLoggerHelper.exe
# Phony target to clean the project
.PHONY: clean
# clean:
# 	@rm -rf $(OBJ_DIR) $(BIN_DIR)

# Ensure header files are considered as dependencies
# -include $(OBJ:.o=.d)

# Rule to generate dependency files
# $(OBJ_DIR)/%.d: $(SRC_DIR)/%.cpp
# 	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -MM -MT '$(OBJ_DIR)/$*.o $@' $< > $@

# Remove dependency files during clean
clean:
	@if exist $(OBJ_DIR) del /Q $(OBJ_DIR)\*.o
	@if exist $(BIN_DIR) del /Q $(BIN_DIR)\*.exe

# Make the 'all' target the default target
.DEFAULT_GOAL := all
