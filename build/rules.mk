o/$(MODE)/%.o:		%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -o $@ $<

o/$(MODE)/%.a:
	$(AR) rcs $@ $^
