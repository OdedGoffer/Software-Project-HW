CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LDFLAGS = -lm -Wno-unused-command-line-argument
SRCDIR = src
OBJDIR = build
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

#TEST
TESTDIR = test
ALLTESTS := $(wildcard test/*.c)
INT = $(TEST:%=$(TESTDIR)/$(OBJDIR)/%)
TESTTARGETS = $(INT)
ifeq ($(TESTTARGETS),)
TESTTARGETS = $(ALLTESTS:$(TESTDIR)/%_test.c=$(TESTDIR)/$(OBJDIR)/%)
endif
TESTDEPS := $(ALLTESTS:$(TESTDIR)/%_test.c=$(OBJDIR)/%.o)
TESTOBJS:= $(ALLTESTS:$(TESTDIR)/%.c=$(TESTDIR)/$(OBJDIR)/%.o)

TARGET = spkmeans

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(info linking $(TARGET)...)
	@$(CC) $(LDFLAGS) -o $@ $^
	$(info ****** SPKMEANS COMPILED SUCCESFULLY! ******)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	-@mkdir -p $(OBJDIR)
	$(info $<...)
	@$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<
	$(info ok)

clean:
	@rm -rf $(OBJDIR)/* $(TARGET)

run: $(TARGET)
	$(info Running...)
	@./$(TARGET)

#TEST
test: $(TESTTARGETS)
	-@rm -rf $(TESTDIR)/$(OBJDIR)

$(TESTDIR)/$(OBJDIR)/%: $(TESTDIR)/$(OBJDIR)/%_test.o $(OBJDIR)/%.o
	@$(CC) $(LDFLAGS) -o $@  $^
	-@./$@

$(OBJDIR)/%.o: $(TESTDIR)/%_test.c
	@$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

$(TESTDIR)/$(OBJDIR)%.o: $(TESTDIR)/%.c
	-@mkdir -p $(TESTDIR)/$(OBJDIR)
	@$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<
