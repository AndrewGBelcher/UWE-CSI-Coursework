# File: Makefile
# Author: Benedict R. Gaster
# Date: 26-06-2017
#
# Copyright: University of West of England 2017
#
ifeq ($(origin ROOTDIR), undefined)
	ROOTDIR=../..
endif
include $(ROOTDIR)/MakefileDefs.inc

# Build path
BUILD_DIR = build

TARGET = libzombies.a
CPP_SOURCES = WorldLoader.cpp ItemInfo.cpp RoomInfo.cpp EntranceInfo.cpp ZombieServer.cpp
H_INSTALL_SOURCES = WorldLoader.h ItemInfo.h RoomInfo.h EntranceInfo.h ZombieServer.h ZombieBot.h
H_SOURCES = $(H_INSTALL_SOURCES)

#######################################
# build the application
#######################################
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

CFLAGS += -I$(INCDIR) -I$(JUCEINSTALL)/modules -I$(BOOSTINSTALL) -I$(WEBSCOCKETINSTALL)

LIBS = $(JUCELIB)

$(BUILD_DIR)/%.o: %.cpp Makefile $(H_SOURCES) | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJECTS) Makefile
	$(AR) rcs $@ $(OBJECTS) $(LDFLAGS)

install: $(BUILD_DIR)/$(TARGET) $(H_INSTALL_SOURCES)
	$(CP) $(BUILD_DIR)/$(TARGET) $(LIBDIR)/$(TARGET)
	-for d in $(H_INSTALL_SOURCES); do ($(CP) $$d  $(INCDIR)/zombies/$$d); done

$(BUILD_DIR):
	mkdir -p $@

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

.PHONY: clean all
