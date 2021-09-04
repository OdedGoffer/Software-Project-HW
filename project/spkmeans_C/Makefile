CC = gcc
CFLAGS = -g -ansi -Wall -Wextra -Werror -pedantic-errors
LDFLAGS = -g -Wno-unused-command-line-argument
LMFLAG = -lm
SRCDIR = src
OBJDIR = build
MAINDIR = main
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
ARGS ?= ddg 3 filelele

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
TARGETOBJ = $(OBJDIR)/$(TARGET).o

.PHONY: all clean run test

all: $(TARGET)
	$(info ****** SPKMEANS COMPILED SUCCESFULLY! ******)

$(TARGET): $(OBJS) $(TARGETOBJ)
	$(info linking $(TARGET)...)
	@$(CC) $(LDFLAGS) -o $@ $^ $(LMFLAG)

$(TARGETOBJ): $(MAINDIR)/$(TARGET).c
	-@mkdir -p $(OBJDIR)
	$(info $<...)
	@$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<
	$(info ok)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	-@mkdir -p $(OBJDIR)
	$(info $<...)
	@$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<
	$(info ok)

clean:
	@rm -rf $(OBJDIR)/* $(TARGET)

run: $(TARGET)
	$(info Running...)
	$(info )
	@./$(TARGET) $(ARGS)

#TEST
test: $(TESTTARGETS)
	-@rm -rf $(TESTDIR)/$(OBJDIR)

$(TESTDIR)/$(OBJDIR)/%: $(TESTDIR)/$(OBJDIR)/%_test.o $(OBJS)
	@$(CC) $(LDFLAGS) -o $@  $^ $(LMFLAG)
	-@./$@

$(OBJDIR)/%.o: $(TESTDIR)/$(OBJDIR)/%_test.o
	-@mkdir -p $(OBJDIR)
	@$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<

$(TESTDIR)/$(OBJDIR)%.o: $(TESTDIR)/%.c
	-@mkdir -p $(TESTDIR)/$(OBJDIR)
	@$(CC) -c $(CFLAGS) $(LDFLAGS) -o $@ $<
