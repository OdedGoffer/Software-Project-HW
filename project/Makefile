CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LDFLAGS = -Wno-unused-command-line-argument
LMFLAG = -lm
SRCDIR = src
INCLDIR = include
OBJDIR = build
PYDIR = python
MAINDIR = main
RELEASEDIR = release
SRCNAMES := logger vector_utils matrix_utils parse_file WAM DDG LNORM jacobi_utils jacobi eigengap kmeans
SRCS := $(SRCNAMES:%=$(SRCDIR)/%.c)
HEADERS := $(SRCNAMES:%=$(INCLDIR)/%.h)
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
ARGS ?= ddg 3 filelele

#TEST VARS
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

.PHONY: all clean run test python release build_release create_release_files

all: $(TARGET) python
	$(info ****** SPKMEANS COMPILED SUCCESFULLY! ******)

$(TARGET): $(OBJS) $(TARGETOBJ)
	$(info linking $(TARGET)...)
	@$(CC) $(LDFLAGS) -o $(PYDIR)/$@ $^ $(LMFLAG)

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
	-@rm -rf $(OBJDIR) $(TARGET) $(RELEASEDIR)
	-@rm python/*.so
	-@rm python/$(TARGET)

run: $(TARGET)
	$(info Running...)
	$(info )
	@./$(TARGET) $(ARGS)


#PYTHON
python: %.so
	-@ mv *.so python/

%.so: $(PYDIR)/setup.py
	@python3 $< build_ext --inplace
	-@rm -rf $(OBJDIR)/temp* $(OBJDIR)/lib*


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

#RELEASE
release: build_release
	$(info ****** RELEASE BUILD SUCCESSFUL! ******)

build_release: create_release_files
	@$(CC) $(CFLAGS) $(RELEASEDIR)/$(TARGET).c $(LMFLAG) -o $(RELEASEDIR)/$(TARGET)
	@cd $(RELEASEDIR) && python3 setup.py build_ext --inplace && rm -rf build

create_release_files: $(SRCS) $(MAINDIR)/$(TARGET).c
	@mkdir -p $(RELEASEDIR)
	@touch $(RELEASEDIR)/temp1
	@touch $(RELEASEDIR)/temp2
	@touch $(RELEASEDIR)/temp3
	@touch $(RELEASEDIR)/$(TARGET).h
	@touch $(RELEASEDIR)/$(TARGET).c
	@cat $^ > $(RELEASEDIR)/temp1
	@cat $(HEADERS) > $(RELEASEDIR)/temp3
	@sed "/^#/d" $(RELEASEDIR)/temp1 > $(RELEASEDIR)/temp2
	@sed "/^#/d" $(RELEASEDIR)/temp3 > $(RELEASEDIR)/$(TARGET).h
	@cat $(INCLDIR)/project_includes.h $(RELEASEDIR)/temp2 > $(RELEASEDIR)/$(TARGET).c
	-@rm $(RELEASEDIR)/temp*
	@cp $(PYDIR)/setup_release.py $(RELEASEDIR)
	@mv $(RELEASEDIR)/setup_release.py $(RELEASEDIR)/setup.py
	@cp $(PYDIR)/spkmeans.py $(RELEASEDIR)
	@cp $(PYDIR)/spkmeansmodule.c $(RELEASEDIR)

