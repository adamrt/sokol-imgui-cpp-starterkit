TARGET=starterkit

CXX = gcc

# Debug build configuration
CXXFLAGS_DEBUG = -Wall -Wextra -Werror -Wpedantic -g \
                 -Wnull-dereference -Wshadow -Wformat=2 \
                 -Wuninitialized -std=c++11
LDFLAGS_DEBUG = -fsanitize=address -fsanitize=undefined

# Release build configuration
CXXFLAGS_RELEASE = -Wall -Wextra -Wpedantic -O3 \
                   -Wnull-dereference -Wshadow -Wformat=2 \
                   -DNDEBUG -std=c++11
LDFLAGS_RELEASE =

LDLIBS = -lGL -ldl -lm -lX11 -lXi -lXcursor -lstdc++ -lglfw
INCLUDES = -Ilib/imgui -Ilib/sokol -Ilib/sokol/util -Ilib/glm -Ilib/stb

SRC_FILES = src/main.cpp src/Camera.cpp src/GUI.cpp src/Shader.cpp src/ResourceManager.cpp src/Pipeline.cpp src/Renderer.cpp src/Model.cpp src/Scene.cpp src/Texture.cpp src/Mesh.cpp src/State.cpp src/utils.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)

IMGUI_CXXFLAGS=-std=c++11 -Ilib/imgui -Ilib/imgui/backends
IMGUI_SOURCES=$(wildcard lib/imgui/*.cpp) \
				lib/imgui/backends/imgui_impl_glfw.cpp \
				lib/imgui/backends/imgui_impl_opengl3.cpp
IMGUI_OBJECTS=$(IMGUI_SOURCES:.cpp=.o)

debug: CXXFLAGS = $(CXXFLAGS_DEBUG)
debug: LDFLAGS = $(LDFLAGS_DEBUG)
debug: $(TARGET)

# Release build target
release: CXXFLAGS = $(CXXFLAGS_RELEASE)
release: LDFLAGS = $(LDFLAGS_RELEASE)
release: $(TARGET)

$(TARGET): $(OBJ_FILES) $(IMGUI_OBJECTS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDLIBS) $(LDFLAGS) $(INCLUDES)

$(IMGUI_OBJECTS): %.o: %.cpp
	$(CXX) -c $< -o $@ $(IMGUI_CXXFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(LDLIBS) $(LDFLAGS) $(INCLUDES)

clean:
	@rm -f src/*.o
	@rm -f $(TARGET)

clean-all: clean
	@rm -f lib/imgui/*.o

sokol-shdc:
	wget -q https://github.com/floooh/sokol-tools-bin/raw/master/bin/linux/sokol-shdc
	chmod +x sokol-shdc

shader: sokol-shdc
	./sokol-shdc -i src/shader.glsl -o src/shader.glsl.h -l glsl430

bootstrap: sokol-shdc shader
	git submodule update --init --recursive

.PHONY: bootstrap clean
