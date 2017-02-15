include ../Makefile.inc

OUT_DIR = ../build/examples
OBJS = $(OUT_DIR)/serialexample.o

all: $(OUT_DIR)/serialexample

clean:
	$(RM) $(OBJS)
	$(MAKE) -C .. clean

$(OUT_DIR)/serialexample: $(OBJS) $(LIBSPT_LIB_FILE) $(OUT_DIR)
	$(CC) -o $@ $(OBJS) $(LIBSPT_LIB_FILE) $(LDFLAGS)

$(OUT_DIR)/serialexample.o: serialexample.c $(OUT_DIR)
	$(CC) $(CFLAGS) $(LIBSPT_INCLUDES) -c $< -o $@

$(LIBSPT_LIB_FILE):
	$(MAKE) -C .. $@

$(OUT_DIR):
	$(MKDIR) -p $@
