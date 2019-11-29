########################################
##
##  This makefile is for building 
##  the ray marcher as a shared object 
##  for testing.
##
########################################

# Compile Info
CC := clang++
SRCEXT := cpp
CFLAGS := -O3 -Wall -std=gnu++17 -fvisibility=hidden -fPIC -fsized-deallocation
CFLAGS += $(shell echo `python3 -m pybind11 --includes`)
CFLAGS += -DBUILDING_FOR_PYTHON_TESTING
# use -isystem on vender headers to suppress warning
INCDIR := -I ../include -I ../vendor/include

#
SOEXT := $(shell echo `python3-config --extension-suffix`)

#
SRCDIR := ../src
BINDIR := bin
TARGET := _rayMarcher$(SOEXT)
SOURCES := $(shell find $(SRCDIR) -type f -iname "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%, $(BINDIR)/%, $(SOURCES:.$(SRCEXT)=.o))

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo "$(CC) $^ $(CFLAGS) -shared -o $(TARGET)"; $(CC) $^ $(CFLAGS) -shared -o $(TARGET)

$(BINDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BINDIR)
	@echo "$(CC) $(CFLAGS) $(INCDIR) -c -o $@ $<"; $(CC) $(CFLAGS) $(INCDIR) -c -o $@ $<

# Clean all binary files
clean:
	@echo " Cleaning...";
	@echo "$(RM) -r $(BINDIR) $(TARGET)"; $(RM) -r $(BINDIR) $(TARGET)

test:
	pytest test.py -v -s

# Declare as utility, not a file
.PHONY: clean test