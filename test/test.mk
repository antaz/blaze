PKGS += TEST_BLAZE
TEST_BLAZE_FILES := $(wildcard test/blink/*)
TEST_BLAZE_SRCS = $(filter %.c,$(TEST_BLAZE_FILES))
TEST_BLAZE_HDRS = $(filter %.h,$(TEST_BLAZE_FILES))
TEST_BLAZE_OBJS = $(TEST_BLAZE_SRCS:%.c=o/%.o)
