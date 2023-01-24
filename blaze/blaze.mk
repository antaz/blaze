PKGS += BLAZE
BLAZE_FILES := $(wildcard blaze/*)
BLAZE_SRCS = $(filter %.c,$(BLAZE_FILES))
BLAZE_HDRS = $(filter %.h,$(BLAZE_FILES))

o/blaze/blaze.a: $(filter-out %/blaze.o,$(BLAZE_SRCS:%.c=o/%.o))

o/blaze/blaze: o/blaze/blaze.o o/blaze/blaze.a
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) -o $@

o/blaze:		\
	o/blaze/blaze	\
