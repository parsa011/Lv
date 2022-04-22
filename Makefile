CC       = gcc
CFLAGS   = -g -I.
SRCDIR   = src src/tui src/builtins libs
OBJDIR   = obj
BINDIR   = bin
TARGET	 = lv

SOURCES  = $(shell find $(SRCDIR) -type f -name '*.c') # main.c say_hello.c
OBJECTS  = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.c=.o)) # main.o say_hello.o
DEPS     = $(OBJECTS:.o=.d)
BINARY   = $(BINDIR)/$(TARGET)
	#-include $(DEPS)

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

clean:
	echo "clean runing..."
	@rm -rvf src/*.o
	@rm -vf bin/* obj/*
