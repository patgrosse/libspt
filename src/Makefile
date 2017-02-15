include ../Makefile.inc
include Makefile.inc

ifdef NO_LIBEVENT
CFLAGS += -DNO_LIBEVENT=1
endif

all: $(LIBSPT_OBJS)

$(LIBSPT_OBJS_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(LIBSPT_INCLUDES) -c $< -o $@

$(LIBSPT_OBJS): | $(LIBSPT_OBJS_DIR)

$(LIBSPT_OBJS_DIR):
	mkdir -p $(LIBSPT_OBJS_DIR)

clean:
	-$(RM) -f $(LIBSPT_OBJS_DIR)/*
