CC       =  gcc
CFLAGS   =  -g -I.
SRCDIR   =  src src/tui src/builtins ../libs
OBJDIR   =  obj
BINDIR   =  bin

SOURCES  = $(shell find $(SRCDIR) -type f -name '*.c') # main.c say_hello.c
OBJECTS  = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.c=.o)) # main.o say_hello.o
DEPS     = $(OBJECTS:.o=.d)
BINARY   = $(BINDIR)/lv
	#-include $(DEPS)

all: buildsolution

buildsolution: dir $(BINARY)

dir:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -MMD -MP -o $@ $<

clean:
	echo "clean runing..."
