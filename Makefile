######################################
##	   WILL'S ULTIMATE MAKEFILE     ##
##         DATE: FEB 2016           ##
######################################
# Compiler
CC = g++
OPTS = -c -g -std=c++11 -Wall

# Project name
PROJECT = $(EXECDIR)/game

# Directories
EXECDIR = bin
OBJDIR = obj
SRCDIR = src
INCDIR = include

# Libraries
LIBS = -L /usr/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INC = -I /usr/include -I $(INCDIR)

# Files and folders
SRCS    = $(shell find $(SRCDIR) -name '*.cc')
SRCDIRS = $(shell find . -name '*.cc' | dirname {} | sort | uniq | sed 's/\/$(SRCDIR)//g' )
OBJS    = $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(SRCS))

# Targets
$(PROJECT): buildrepo $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	$(CC) $(OPTS)  $(INC) -c $< -o $@
	
clean:
	rm $(PROJECT) $(OBJDIR) -Rf
	
buildrepo:
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef
