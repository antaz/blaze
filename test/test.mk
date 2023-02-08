o/$(MODE)/test:	o/$(MODE)/test/perft

o/$(MODE)/test/perft: o/$(MODE)/test/perft.o o/$(MODE)/blaze/blaze.a
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) -o $@
