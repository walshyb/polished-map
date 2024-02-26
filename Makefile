# Compiler settings
CXX := em++
CXXFLAGS := -std=c++17 --bind -s USE_ZLIB=1 -s USE_LIBPNG=1
EM_FLAGS := -s EXPORTED_RUNTIME_METHODS=ccall,allocateUTF8,cwrap -s EXPORTED_FUNCTIONS=_free,_main,_processFile,_getBlocks,_readMetatileData,_drawMetatile

# Source directory
SRC_DIR := core

# Output directory
OUT_DIR := public

# Get list of .cpp files
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")

# Output file
OUT_FILE := $(OUT_DIR)/main.js

# Main target
.PHONY: all
all:$(OUT_FILE)

# Rule to compile .cpp files to output file
$(OUT_FILE): $(SRCS)
	$(CXX) $^ -o $@ $(EM_FLAGS) $(CXXFLAGS)
