CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
#LDFLAGS = -lm
SRCDIR = src
OBJDIR = build
TESTDIR = test
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=$(OBJDIR)/%.o)
TESTS := $(wildcard test/*.c)

TARGET = spkmeans

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(info linking $(TARGET)...)
	@$(CC) $(LDFLAGS) -o $@ $^
	$(info ****** SPKMEANS COMPILED SUCCESFULLY! ******)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	-@mkdir -p $(OBJDIR)
	$(info compiling $@)
	@$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	@rm -rf $(OBJDIR)/* $(TARGET)

run: $(TARGET)
	$(info Running...)
	@./$(TARGET)

test: $(TESTS)
	$(info TESTING: $<)
	@gcc $(CFLAGS) $< -o $(TESTDIR)/tmp
	-@./$(TESTDIR)/tmp
	-@rm $(TESTDIR)/tmp