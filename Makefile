TARGET_EXEC ?= video-editor

BUILD_DIR ?= ./
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := ./include/
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= -O1 -Wall -std=c99 -Wno-missing-braces -Wall -Wextra $(INC_FLAGS) -MMD -MP

LDFLAGS ?= -L ./lib/ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
