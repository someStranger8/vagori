CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude -Ilib
LDFLAGS = -lSDL2 -lSDL2_image -llua -lm -lpthread -ldl

# Source files
SRCS = src/main.c \
       src/engine.c \
       src/config.c \
       src/audio.c \
       src/inventory.c \
       src/scene.c \
       src/lua_bindings.c \
       lib/cJSON.c

# Map source files to the obj/ directory (e.g., src/main.c -> obj/src/main.o)
OBJS = $(patsubst %.c, obj/%.o, $(SRCS))
TARGET = game_engine

all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files into obj/ preserving directory structure
obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj $(TARGET)

.PHONY: all clean
