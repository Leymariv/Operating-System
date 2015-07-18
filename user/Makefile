#
# Ensimag - Projet système
# Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
#
# User space compile tool chain.
# This Makefile use sub makefiles in build/ to build and link each app and
# the standard library for our custom kernel/userspace environment.

### Tools ###

## PREFIX ease the use of non standard build chain tools (MacOS X)
## PREFIX := /usr/local/i386-pc-elf/bin/i386-pc-elf-

CC := $(PREFIX)gcc
AS := $(PREFIX)gcc
LD := $(PREFIX)ld.bfd
AR := $(PREFIX)ar
DEP := $(PREFIX)gcc
DEPS := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
NM=$(PREFIX)nm 
MKDIR := mkdir
CP := cp
DEPS := gcc

### Override with quiet tools by default
ifneq "$(VERBOSE)" "1"
include build/quiet.mk
endif


# Global compilation flags
CFLAGS := -m32 \
		  -Wall -Wextra -Werror -Wmissing-include-dirs -std=c99 \
		  -g -gstabs \
		  -pipe \
		  -nostdinc \
		  -fno-stack-protector
LDFLAGS := -melf_i386 \
	-nostdlib


# Destination directory for compilation products
OUTPUT := out

# Include makefile utilities
include build/utils.mk

### Import standard library build toolchain ###
STDLIB_DIRS := ../shared lib
STDLIB_NAME := std
include build/stdlib.mk

### Compile userspace tests ###
TESTS_DIR     := tests
TESTS_OPTIONS := CONS_READ_LINE WITH_MSG
include build/tests.mk

### Import userspace program build toolchain ###
APPS_NAMES      := test_anneau_jeton test_anneau_jeton1 test_return test_return1 test_sleep_kill test_sleep_kill1 test_exit test_waitpid test_waitpid1 test_shm test_shm1 test_sem test_sem2 test_void test_privilege test_privilege1 shell test_ps test_ps1 test_sinfo test_sinfo1 editor test_malloc test_gdf help
APPS_STD_INC    := $(STDLIB_DIRS)
APPS_STD_LIB    := $(OUTPUT)/lib$(STDLIB_NAME).a
APPS_STD_TARGET := $(STDLIB_TARGET)
include build/apps.mk

# The all target is virtual, it is just a way to call all
.PHONY: all
all: $(APPS_TARGETS) $(TESTS_TARGETS)

.PHONY: clean
clean:
	rm -rf $(OUTPUT)
