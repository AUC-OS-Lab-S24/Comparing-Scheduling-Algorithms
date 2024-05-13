# Compiler settings - Can be customized.
CXX = g++
CXXFLAGS = -Wall -g -Iinclude

# Define the executable file 
TARGET = myapp

# Find all the C++ files in the src/ directory
SOURCES = $(wildcard src/*.cpp)

# Replace the .cpp from SOURCES with .o to obtain object files.
OBJECTS = $(SOURCES:.cpp=.o)

# Link the object files into a binary
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile the source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJECTS)

# Phony targets
.PHONY: clean
