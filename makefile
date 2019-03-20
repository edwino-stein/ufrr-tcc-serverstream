# General settings
PROJECT_NAME = serverstream
SRC_DIR = src
HEADER_DIR = headers
BUILD_DIR = build
OBJ_EXTENSION = .o
DEPENDENCE_EXTENSION = .d

# Custom includes and libs
INCLUDES = $(HEADER_DIR) includes
MACROS =
STATIC_LIBRARIES =
DLIBRARIES_PATH =
DYNAMIC_LIBRARIES = boost_system dl

# Unit tests settings
EXCLUDE_FILES = main.cpp
TESTS_MK = tests.mk
UTEST_DIR = tests
UTEST_BUILD_DIR = $(BUILD_DIR)/$(UTEST_DIR)
UTEST_LD = boost_unit_test_framework $(DYNAMIC_LIBRARIES)

# Compiler and linker settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -MMD
CXXEXTRAFLAGS = $(addprefix -I, $(INCLUDES)) $(addprefix -D, $(MACROS))
LDLIBS = $(addprefix -l, $(DYNAMIC_LIBRARIES))
LDFLAGS = $(addprefix -L, $(DLIBRARIES_PATH))

# Static library builder
ARRVS = ar rvs

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

# Define set object files for unit tests
EXCLUDE_OBJS = $(addprefix $(BUILD_DIR)/, $(EXCLUDE_FILES))
EXCLUDE_OBJS := $(addsuffix $(OBJ_EXTENSION), $(EXCLUDE_OBJS))
OBJS_UTEST_FILES  = $(filter-out $(EXCLUDE_OBJS), $(OBJS_FILES))

# Set dependencies objects
DEPS = $(OBJS_FILES:$(OBJ_EXTENSION)=$(DEPENDENCE_EXTENSION))

# Set default static library for unit tests
l?=$(BUILD_DIR)/$(PROJECT_NAME)-utest-all.a

# *********************************** RULES ************************************
.PHONY: all clean run test
all: $(PROJECT_NAME)
clean:
	rm -rf $(BUILD_DIR)

#Runtime builder
$(PROJECT_NAME): $(BUILD_DIR)/$(PROJECT_NAME)
$(BUILD_DIR)/$(PROJECT_NAME): $(OBJS_FILES)
	$(CXX) $(CXXFLAGS) -rdynamic $(OBJS_FILES) $(STATIC_LIBRARIES) -o $@ $(LDFLAGS) $(LDLIBS)

# Objects builder
$(BUILD_DIR)/%$(OBJ_EXTENSION): $(SRC_DIR)/%
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CXXEXTRAFLAGS) -c $< -o $@

# Run
run: $(BUILD_DIR)/$(PROJECT_NAME)
	./$(BUILD_DIR)/$(PROJECT_NAME)

# Unit Tests builder
test: $(l)
	@mkdir -p $(UTEST_BUILD_DIR)
	$(MAKE) -f $(TESTS_MK) $(UTEST_BUILD_DIR)/$(notdir $(basename $(t))) ls="$(l) $(ls)" i="$(INCLUDES) $(i)" ld="$(UTEST_LD) $(ld)"
	./$(UTEST_BUILD_DIR)/$(notdir $(basename $(t))) --log_level=all

$(BUILD_DIR)/$(PROJECT_NAME)-utest-all.a: $(OBJS_UTEST_FILES)
	@$(ARRVS) $@ $(OBJS_UTEST_FILES)

# Include all .d files
-include $(DEPS)