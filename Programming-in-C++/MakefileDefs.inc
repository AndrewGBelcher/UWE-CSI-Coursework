# Standard defs

CC	= clang++
PRJCFLAGS	= -g
LD	= clang++
LDFLAGS	= -lcrypt 
CFLAGS=-std=c++14
AR	= llvm-ar
ARFLAGS	=
RANLIB	= ranlib
RM	= rm
ECHO	= echo
CP = cp

SHELL	= /bin/sh

INCDIR=$(ROOTDIR)/include
LIBDIR=$(ROOTDIR)/lib
JUCELIB=$(ROOTDIR)/lib/libjuce5.a
JUCEINSTALL=$(ROOTDIR)/3rdparty/JUCE
LINUXLIBFALGS = $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama  webkit2gtk-4.0 gtk+-x11-3.0) -lGL -ldl -lpthread -lrt

BOOSTINSTALL=/usr/local/boost_1_65_1
BOOSTLIBDIR=/usr/local/boost_1_65_1/stage/lib
WEBSOCKETINSTALL=$(ROOTDIR)/3rdparty/websocketpp

.SILENT :
