AUTOMAKE_OPTIONS = 1.4 foreign
CFLAGS += -Wall
lib_LTLIBRARIES = libnss_tld_mapper.la
libnss_tld_mapper_la_SOURCES = libnss_tld_mapper.c
libnss_tld_mapper_la_CFLAGS = -O2
libnss_tld_mapper_la_LDFLAGS=$(AM_LDFLAGS) -shrext .so.2
libnss_tld_mapper_la_LIBADD = -ldl
