include Makefile.inc

all: $(LIBSPT_LIB_FILE)

objs: $(LIBSPT_OBJS)

clean:
	$(RM) $(LIBSPT_LIB_FILE)
	$(MAKE) -C src clean

$(LIBSPT_LIB_FILE): $(LIBSPT_OBJS)
	$(AR) rcs $(LIBSPT_LIB_FILE) $(LIBSPT_OBJS)

$(LIBSPT_OBJS):
	$(MAKE) -C src $@
