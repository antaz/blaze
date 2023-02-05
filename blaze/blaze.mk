PKGS += BLAZE
BLAZE_FILES := $(wildcard blaze/*)
BLAZE_SRCS = $(filter %.c,$(BLAZE_FILES))
BLAZE_HDRS = $(filter %.h,$(BLAZE_FILES))

o/$(MODE)/blaze/blaze.a: $(filter-out %/blaze.o,$(BLAZE_SRCS:%.c=o/$(MODE)/%.o))

o/$(MODE)/blaze/blaze: o/$(MODE)/blaze/blaze.o o/$(MODE)/blaze/blaze.a
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) -o $@

o/$(MODE)/blaze:			\
	o/$(MODE)/blaze/blaze		\
