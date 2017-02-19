# used commands and flags
export CC ?= gcc
export LDFLAGS += -lpthread
export LD ?= gcc
export AR ?= ar
export RM ?= rm
export MKDIR ?= mkdir
export ECHO ?= echo
export SHELL ?= /bin/sh

# libspt specific variables
export LIBSPT_CFLAGS ?= -g -std=gnu99 -Werror -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wcast-align -Wstrict-prototypes \
	-Wstrict-overflow=5 -Wno-write-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum \
	-Wunreachable-code

export LIBSPT_SRC_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))/src

export LIBSPT_OBJS_DIR ?= $(abspath $(LIBSPT_SRC_DIR)/../build/libspt)

export LIBSPT_LIB_FILE ?= $(abspath $(LIBSPT_SRC_DIR)/../build/libspt/libspt.a)

export LIBSPT_INCLUDES += -I$(LIBSPT_SRC_DIR)/include

export LIBSPT_CFILES := \
			base64.c \
			data_fifo.c \
			spt_logger.c \
			serial_io.c \
			spt.c \
			tty_utils.c

export LIBSPT_HFILES := \
			base64.h \
			data_fifo.h \
			spt_logger.h \
			serial_io.h \
			spt.h \
			tty_utils.h \
			unused.h

export LIBSPT_OBJS = $(addprefix $(LIBSPT_OBJS_DIR)/, $(LIBSPT_CFILES:.c=.o))

# handling of options in environment variables
ifndef __LIBSPT_ENV_VAR_PARSED
    export __LIBSPT_ENV_VAR_PARSED := 1
    ifdef NO_LIBEVENT
        LIBSPT_CFLAGS += -DNO_LIBEVENT=1
    else
        LDFLAGS += -levent
    endif
    ifdef NO_TERMIOS
        LIBSPT_CFLAGS += -DNO_TERMIOS=1
    endif
endif