CC       = gcc
CFLAGS   = -g -I.
SRCDIR   = src libs
OBJDIR   = obj
BINDIR   = bin
TARGET	 = lv

SOURCES  = $(shell find $(SRCDIR) -type f -name '*.c')
INCLUDES = $(shell find $(SRCDIR) -type f -name '*.h')
OBJECTS  = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.c=.o))
DEPS     = $(OBJECTS:.o=.d)
BINARY   = $(BINDIR)/$(TARGET)
#-include $(DEPS)

ifeq ($(PREFIX),)
    PREFIX := /usr/local/bin
endif

all: buildsolution

buildsolution: dir $(BINARY)

dir:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)

$(BINARY): $(OBJECTS)
	@$(CC) $(CFLAGS) $^ -o $@
	@echo "Build Done"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	echo " - Compiling \033[35m$<\033[0m"
	@$(CC) $(CFLAGS) -c -MMD -MP -o $@ $<

.PHONY 	: install
install:
	install -m 557 $(BINDIR)/$(TARGET) $(PREFIX)

clean:
	echo "clean runing..."
	@rm -rvf src/*.o
	@rm -vf bin/* obj/*
