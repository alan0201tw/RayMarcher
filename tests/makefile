##
## Makefile for testing Ray marcher
##

# Compile Info
CC := clang++
SRCEXT := cpp
CFLAGS := -O3 -std=gnu++17 -pthread -g -Wall
# use -isystem on vender headers to suppress warning
INCDIR := -I ../include -I ../vendor/include -Igtest/include
LINKS := -Lgtest/lib -lgtest -lpthread

# Source Info, target = cpplox, entry should be in cpplox.cpp
SRCDIR := ../src
BINDIR := bin
TARGET := test
SOURCES := $(shell find $(SRCDIR) -type f -iname "*.$(SRCEXT)" ! -iname "$(TARGET).$(SRCEXT)" ! -iname "main.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BINDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Compile main
$(TARGET): $(OBJECTS) $(BINDIR)/$(TARGET).o
	@echo "Linking..."
	@echo "$(CC) $^ $(CFLAGS) -o $(TARGET) $(LINKS)"; $(CC) $^ $(CFLAGS) -o $(TARGET) $(LINKS)

$(BINDIR)/$(TARGET).o: $(SRCDIR)/../tests/$(TARGET).$(SRCEXT)
	@mkdir -p $(BINDIR)
	@echo "$(CC) $(CFLAGS) $(INCDIR) -c -o $@ $<"; $(CC) $(CFLAGS) $(INCDIR) -c -o $@ $<

$(BINDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BINDIR)
	@echo "$(CC) $(CFLAGS) $(INCDIR) -c -o $@ $<"; $(CC) $(CFLAGS) $(INCDIR) -c -o $@ $<

# Clean all binary files
clean:
	@echo " Cleaning..."; 
	@echo "$(RM) -r $(BINDIR) $(TARGET)"; $(RM) -r $(BINDIR) $(TARGET)

# Declare clean as utility, not a file
.PHONY: clean
