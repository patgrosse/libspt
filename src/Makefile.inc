LIBSPT_SRC_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

LIBSPT_OBJS_DIR ?= $(abspath $(LIBSPT_SRC_DIR)/../build/libspt)

LIBSPT_INCLUDES += -I$(LIBSPT_SRC_DIR)/include

LIBSPT_CFILES := \
			base64.c \
			data_fifo.c \
			spt_logger.c \
			serial_io.c \
			spt.c \
			tty_utils.c

LIBSPT_HFILES := \
			base64.h \
			data_fifo.h \
			spt_logger.h \
			serial_io.h \
			spt.h \
			tty_utils.h \
			unused.h

LIBSPT_OBJS = $(addprefix $(LIBSPT_OBJS_DIR)/, $(LIBSPT_CFILES:.c=.o))