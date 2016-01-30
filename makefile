SOURCE_FILE = sdl.c
COMPILER = gcc
COMPILER_FLAGS = -w
LINKER_FLAGS = -lSDL2 -lGL -lGLU
OUTPUT_FILE = run

all : $(SOURCE_FILE)
	  $(COMPILER) $(SOURCE_FILE) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OUTPUT_FILE)
