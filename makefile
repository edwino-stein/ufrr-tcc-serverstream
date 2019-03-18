# General settings
PROJECT_NAME = serverstream
SRC_DIR = src
HEADER_DIR = headers
BUILD_DIR = build
OBJ_EXTENSION = .o
DEPENDENCE_EXTENSION = .d

# Custom includes and libs
INCLUDES = $(HEADER_DIR)
MACROS =
STATIC_LIBRARIES =
DLIBRARIES_PATH =
DYNAMIC_LIBRARIES =

# Compiler and linker settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -MMD
CXXEXTRAFLAGS = $(addprefix -I, $(INCLUDES)) $(addprefix -D, $(MACROS))
LDLIBS = $(addprefix -l, $(DYNAMIC_LIBRARIES))
LDFLAGS = $(addprefix -L, $(DLIBRARIES_PATH))

# Defines src path and source file extensions
VPATH = src: $(SRC_DIR)
vpath %.cpp $(SRC_DIR)
vpath %.hpp $(HEADER_DIR)

# Search for all srcs and includes files
SRC_FILES = $(shell find $(SRC_DIR) -type f \( -name "*.cpp" \) )
INCLUDE_FILES = $(shell find $(HEADER_DIR) -type f \( -name "*.hpp" \) )

#Define set object files
OBJS_FILES = $(subst $(SRC_DIR)/,$(BUILD_DIR)/,$(SRC_FILES))
OBJS_FILES := $(addsuffix $(OBJ_EXTENSION), $(OBJS_FILES))

# Set dependencies objects
DEPS = $(OBJS_FILES:$(OBJ_EXTENSION)=$(DEPENDENCE_EXTENSION))

# *********************************** RULES ************************************
.PHONY: all clean run test
all: $(PROJECT_NAME)
clean:
	rm -rf $(BUILD_DIR)

#Runtime builder
$(PROJECT_NAME): $(BUILD_DIR)/$(PROJECT_NAME)
$(BUILD_DIR)/$(PROJECT_NAME): $(OBJS_FILES)
	$(CXX) $(CXXFLAGS) $< $(STATIC_LIBRARIES) -o $@ $(LDFLAGS) $(LDLIBS)

# Objects builder
$(BUILD_DIR)/%$(OBJ_EXTENSION): $(SRC_DIR)/%
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CXXEXTRAFLAGS) -c $< -o $@

# Run
run: $(BUILD_DIR)/$(PROJECT_NAME)
	./$(BUILD_DIR)/$(PROJECT_NAME)

# Include all .d files
-include $(DEPS)