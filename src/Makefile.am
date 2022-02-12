EXTRA_DIST = TODO

# sysconf_lvdir = $(sysconfdir)/lv
# data_lvdir = $(datadir)/lv

noinst_HEADERS = types.h macro.h lv.h utils.h tty.h glob.h utf8.h input.h line.h buffer.h move.h \
					vt100.h display.h window.h file.h color.h word.h undo.h commands.h builtins/builtins.h \
					prompt.h log.h highlight.h ../libs/llist.h  \
                    tui/rec.h 

bin_PROGRAMS = lv 
# INCLUDES = -DLVRC="\"$(sysconf_lvdir)/\"" -DJOEDATA="\"$(data_lvdir)/\""

lv_SOURCES = lv.c tty.c utils.c glob.c utf8.c input.c line.c move.c vt100.c display.c buffer.c	 \
					window.c file.c macro.c word.c commands.c builtins/q.c builtins/w.c builtins/set.c  \
					builtins/o.c builtins/buffer.c prompt.c log.c highlight.c undo.c \
                    tui/rec.c 

# debugger_SOURCES = debugger.c

# stringify_SOURCES = stringify.c

distclean-local:
	rm -f *~

.PHONY: termcap
