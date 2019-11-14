# Compile Info
CC := clang++
SRCEXT := cpp
CFLAGS := -O3 -std=gnu++17 -pthread -g -Wall
# use -isystem on vender headers to suppress warning
INCDIR := -I include -I vendor/include

# Source Info, target = cpplox, entry should be in cpplox.cpp
SRCDIR := src
BINDIR := bin
TARGET := main
SOURCES := $(shell find $(SRCDIR) -type f -iname "*.$(SRCEXT)" ! -iname "$(TARGET).$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BINDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Compile main
$(TARGET): $(OBJECTS) $(BINDIR)/$(TARGET).o
	@echo "Linking..."
	@echo "$(CC) $^ $(CFLAGS) -o $(TARGET) $(LIB)"; $(CC) $^ $(CFLAGS) -o $(TARGET)

$(BINDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BINDIR)
	@echo "$(CC) $(CFLAGS) $(INCDIR) -c -o $@ $<"; $(CC) $(CFLAGS) $(INCDIR) -c -o $@ $<

# Clean all binary files
clean:
	@echo " Cleaning..."; 
	@echo "$(RM) -r $(BINDIR) $(TARGET)"; $(RM) -r $(BINDIR) $(TARGET)
	@echo "$(RM) -r $(TESTBINDIR)"; $(RM) -r $(TESTBINDIR)

remove_output:
	@echo " Removing output...";
	@echo "$(RM) -r output/*"; $(RM) -r output/*
	@echo "$(RM) -r result.mp4"; $(RM) -r result.mp4

video:
	@echo " Making Video...";
	@echo "./util/gen_video.sh output"; ./util/gen_video.sh output

play:
	@echo " Playing Video...";
	@echo "xdg-open result.mp4"; xdg-open result.mp4

# Declare clean as utility, not a file
.PHONY: clean exec video play