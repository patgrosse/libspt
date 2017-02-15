include ../Makefile.inc

all: $(LIBSPT_OBJS)

$(LIBSPT_OBJS_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(LIBSPT_INCLUDES) -c $< -o $@

$(LIBSPT_OBJS): | $(LIBSPT_OBJS_DIR)

$(LIBSPT_OBJS_DIR):
	$(MKDIR) -p $@

clean:
	$(RM) $(LIBSPT_OBJS)
