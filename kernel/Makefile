#
# Ensimag - Projet système
# Copyright (C) 2001-2003 - Simon Nieuviarts
# Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
#

### Tools ###

## PREFIX ease the use of non standard build chain tools (MacOS X)
# PREFIX := /usr/local/i386-pc-elf/bin/i386-pc-elf-
CC := $(PREFIX)gcc
AS := $(PREFIX)gcc
LD := $(PREFIX)ld.bfd
AR := $(PREFIX)ar
DEP := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
NM=$(PREFIX)nm 
MKDIR := mkdir
CP := cp
GEN_SECTIONS := build/generate-link-sections.sh
GEN_TABLE := build/generate-symbols-table.sh

### Override with quiet tools by default
ifneq "$(VERBOSE)" "1"
include build/quiet.mk
endif

### Tools flags ###
DEFS   := -D__KERNEL__
CFLAGS := -m32 \
		  -Wall -Wextra -Werror -std=c99 \
		  -g -gstabs \
		  -pipe \
		  -nostdinc \
		  -nostdlib \
		  -fno-stack-protector \
		  $(DEFS)
ASFLAGS := -m32 \
		   -DASSEMBLER \
		   $(DEFS)
ARFLAGS := rcs
LDFLAGS := -melf_i386

# Import facilities
include build/utils.mk

### Compilation sources ###
# Use DIRS variable to add source sub-directories
DIRS := .
SRC  :=	\
	$(call all-asm-files-under, $(DIRS)) \
	$(call all-c-files-under , $(DIRS))
INCLUDES := $(addprefix -I, $(DIRS) ../shared)

### Output files ###
OUTPUT      := out
DEPS        := $(addprefix $(OUTPUT)/, $(call generate-deps, $(SRC)))
OBJS        := $(addprefix $(OUTPUT)/, $(call objetize, $(SRC))) \
	$(OUTPUT)/task_dump_screen.o
OUTPUT_DIRS := $(addprefix $(OUTPUT)/, $(DIRS))

### Shared lib build ###
SHARED_LIB_DIR := ../shared
include build/shared-lib.mk


### Linux keyboard support build ###
KBD_LINUX_DIR := kbd-linux
include build/kbd-linux.mk


### Build code for real-mode ###
#REAL_MODE_DIR := real-mode
#include build/real-mode.mk

### Generate object files for userspace binaries ###
include build/apps-integration.mk


### Generate the kernel, ready to run ###
kernel.bin: build/kernel.lds $(OBJS) $(SHARED_LIB_TARGET) $(KBD_LINUX_TARGET) $(APPS_OBJS) $(REAL_MODE_TARGET)
	$(LD) $(LDFLAGS) -e entry -T$< $(filter-out %.lds, $^) -o $@


### Generic rules ###
$(OUTPUT)/%.d: %.c | $(OUTPUT)
	@printf "$(@D)/" > $@
	$(DEP) $(INCLUDES) -MM $< >> $@

$(OUTPUT)/%.d: %.S | $(OUTPUT)
	@printf "$(@D)/" > $@
	$(DEP) $(INCLUDES) -MM $< >> $@

$(OUTPUT)/%.o: %.c | $(OUTPUT)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OUTPUT)/%.o: %.S | $(OUTPUT)
	$(AS) $(ASFLAGS) $(INCLUDES) -c $< -o $@

# A text screen for exception dump
$(OUTPUT)/task_dump_screen.o: $(OUTPUT)/empty.o task_dump_screen.txt
	$(OBJCOPY) $< \
		--add-section=.task_dump_screen=$(filter-out $<, $^) \
		--set-section-flags=.task_dump_screen=contents,alloc,load,data $@


# Add a dependency between each object file and the directory that will contain
# it.
$(foreach obj-file, $(OBJS), $(eval 			\
	$(call generate-build-dir-dep, $(obj-file)) \
))
# Create a target for each build output directory
$(foreach build-dir, $(OUTPUT_DIRS), $(eval 		\
	$(call generate-build-dir-target, $(build-dir))	\
))

# Include app dependency files if required
ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPS)
endif


### Build output directory ###
$(OBJS): | $(OUTPUT)
$(DEPS): | $(OUTPUT)
$(OUTPUT):
	$(MKDIR) -p $@


### Clean everything ###
.PHONY: clean
clean:
	rm -rf $(OUTPUT) kernel.bin

