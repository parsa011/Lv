EXTRA_DIST = TODO

# sysconf_lvdir = $(sysconfdir)/lv
# data_lvdir = $(datadir)/lv

noinst_HEADERS = types.h macro.h lv.h

bin_PROGRAMS = lv
# INCLUDES = -DLVRC="\"$(sysconf_lvdir)/\"" -DJOEDATA="\"$(data_lvdir)/\""

lv_SOURCES = lv.c

# termidx_SOURCES = termidx.c

# stringify_SOURCES = stringify.c

distclean-local:
	rm -f *~

.PHONY: termcap
