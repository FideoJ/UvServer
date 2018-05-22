CC := g++

BINDIR := bin
SRCDIR := src
BUILDDIR := build
TARGET := $(BINDIR)/UvServer

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
LIB := -luv
INC := -I include

$(TARGET) : $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $^ $(LIB) -o $(TARGET)

$(BUILDDIR)/%.o : $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(INC) -c -o $@ $<

clean:
	rm -rf $(BUILDDIR) $(TARGET)

.PHONY: clean
