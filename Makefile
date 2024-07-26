######################################
##	   WILL'S ULTIMATE MAKEFILE     ##
##         DATE: FEB 2016           ##
######################################
# Compiler
CC = g++
OPTS = -c -g -Wall

# Project name
PROJECT = $(EXECDIR)/game
TESTS = $(EXECDIR)/tests

# Directories
EXECDIR = bin
OBJDIR = obj
SRCDIR = src
INCDIR = include
TESTDIR = tests

# Libraries
LIBS = -L /usr/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INC = -I /usr/include -I $(INCDIR)

# Files and folders
SRCS    = $(shell find $(SRCDIR) -name '*.cc' -not -path '*tests*')
TESTSRCS = $(shell find $(SRCDIR) -name '*.cc' ! -name '*main.cc')
SRCDIRS = $(shell find . -name '*.cc' | dirname {} | sort | uniq | sed 's/\/$(SRCDIR)//g' )
OBJS    = $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(SRCS))
TESTOBJS = $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(TESTSRCS))

# Targets
$(PROJECT): buildrepo $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $@

$(TESTS): $(TESTOBJS)
	$(CC) $(TESTOBJS) $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	$(CC) $(OPTS) $(INC) -c $< -o $@
	
clean:
	rm $(PROJECT) $(OBJDIR) $(TESTS) -Rf
	
buildrepo:
	@$(call make-repo)

tests: buildrepo $(TESTS)

	

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/$(TESTDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef