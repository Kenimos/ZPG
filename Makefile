# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++14 -Wall -IHeaders -I/opt/homebrew/include

# Linker flags (for OpenGL, GLEW, and GLFW)
LDFLAGS = -L/opt/homebrew/lib -lGLEW -lglfw -framework OpenGL

# Folders
SRC_DIR = Cpp
HEADER_DIR = Headers
SHADER_DIR = Shaders
BUILD_DIR = build

# Output executable (will be placed in the root directory)
TARGET = main

# Source files (include main.cpp explicitly)
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) main.cpp

# Object files
OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SOURCES)))

# Default target
all: $(TARGET)

# Create output binary in the root directory
$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile object files
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADER_DIR)/%.h
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean the build directory and the main executable
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Run the program
run: all
	./$(TARGET)
