CC = gcc
CFLAGS = -Wall -Wextra -g -pthread

SRC_DIR = source
INC_DIR = headerfiles
OBJ_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/program

# Find C sources, including files in subdirectories.
SOURCES := $(shell find $(SRC_DIR) -type f -name '*.c' 2>/dev/null)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

CPPFLAGS += -I$(INC_DIR)

.PHONY: all clean
.DELETE_ON_ERROR:

all: $(TARGET)

# Link all object files into one executable.
$(TARGET): $(OBJECTS)
	mkdir -p $(@D)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

# Compile each source and record its header dependencies.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)
	$(RM) $(TARGET)

-include $(DEPS)
