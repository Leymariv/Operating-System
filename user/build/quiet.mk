#
# Ensimag - Projet système
# Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
#

# Use temporary variables to store the tools
QCC := $(CC)
QAS := $(AS) 
QLD := $(LD)
QAR := $(AR)
QCP := $(CP)
QDEPS := $(DEPS)
QMKDIR := $(MKDIR)
QOBJCOPY := $(OBJCOPY)
QGENSECTIONS := $(GEN_SECTIONS)
QGENTABLE := $(GEN_TABLE)

# Define a quiet version of each tool
# If make version >= 3.82 we need the -e flag
ifeq ($(firstword $(sort $(MAKE_VERSION) 3.82)),3.82)
ECHO_FLAG := -e
else
ECHO_FLAG :=
endif


CC = @echo $(ECHO_FLAG) "    CC [U]\t $@"; $(QCC)
AS = @echo $(ECHO_FLAG) "    AS [U]\t $@"; $(QAS)
LD = @echo $(ECHO_FLAG) "    LD [U]\t $@"; $(QLD)
AR = @echo $(ECHO_FLAG) "    AR [U]\t $@"; $(QAR)
DEPS = @echo $(ECHO_FLAG) "    DEPS [U]\t $@"; $(QDEPS)
CP = @echo $(ECHO_FLAG) "    CP\t\t $< -> $@"; $(QCP)
MKDIR = @echo $(ECHO_FLAG) "    MKDIR\t $@"; $(QMKDIR)
OBJCOPY =  @echo $(ECHO_FLAG) "    OBJCOPY\t $@"; $(QOBJCOPY)


