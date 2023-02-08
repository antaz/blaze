SRCS = $(wildcard blaze/*.c)
INCS = $(wildcard blaze/*.h)

o/$(MODE)/blaze/blaze.a: $(filter-out %/blaze.o,$(SRCS:%.c=o/$(MODE)/%.o))

o/$(MODE)/blaze/blaze: o/$(MODE)/blaze/blaze.o o/$(MODE)/blaze/blaze.a
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) -o $@

o/$(MODE)/blaze: o/$(MODE)/blaze/blaze
