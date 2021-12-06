EXTRA_DIST = TODO

# sysconf_lvdir = $(sysconfdir)/lv
# data_lvdir = $(datadir)/lv

noinst_HEADERS = types.h macro.h lv.h utils.h tty.h glob.h utf8.h input.h line.h buffer.h move.h \
					vt100.h display.h window.h

bin_PROGRAMS = lv
# INCLUDES = -DLVRC="\"$(sysconf_lvdir)/\"" -DJOEDATA="\"$(data_lvdir)/\""

lv_SOURCES = lv.c tty.c utils.c glob.c utf8.c input.c line.c move.c vt100.c display.c \
					buffer.c window.c

# termidx_SOURCES = termidx.c

# stringify_SOURCES = stringify.c

distclean-local:
	rm -f *~

.PHONY: termcap
