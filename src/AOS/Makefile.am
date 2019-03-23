## Process this file with automake to produce Makefile.in

#AUTOMAKE_OPTIONS = no-dependencies

INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/src

if HAVE_AMIGAOS
# Build a libtool convenience library.
noinst_LTLIBRARIES = libMesaAOS.la
endif

libMesaAOS_la_SOURCES =		\
 	amesatools.c		\
	amigamesa.c 		\
	cmnDisplay.c 		\
	natDisplay.c 		\
	natDisplayExtra.c	\
	cybDisplay.c		\
	cybDisplayExtra.c	\
	wrpDisplay.c		\
	palettes.c 		\
	mempools.c
