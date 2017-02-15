include ../Makefile.inc
include ../src/Makefile.inc

OUT_DIR = ../build
OBJS = $(OUT_DIR)/serialexample.o

LDFLAGS += -lpthread
ifndef NO_LIBEVENT
LDFLAGS += -levent
CFLAGS += -DNO_LIBEVENT=1
endif

all: $(OUT_DIR)/serialexample

$(OUT_DIR)/serialexample: $(OBJS) $(LIBSPT_OBJS)
	$(CC) -o $@ $(OBJS) $(LIBSPT_OBJS) $(LDFLAGS)

$(OUT_DIR)/serialexample.o: serialexample.c
	$(CC) $(CFLAGS) $(LIBSPT_INCLUDES) -c $< -o $@

$(LIBSPT_OBJS):
	@echo $(LIBSPT_OBJS)
	$(MAKE) -C ../src $(LIBSPT_OBJS)

clean:
	$(ECHO) cleaning up in $(shell pwd)
	-$(RM) -f $(OBJS)
	$(MAKE) -C ../src clean
