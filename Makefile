ifneq ($(m),)
ifeq ($(MODE),)
MODE := $(m)
endif
endif

.PHONY: o all clean test tags

o:      o/$(MODE)/blaze

test:   o       \
        o/$(MODE)/test

tags: TAGS

clean:
	rm -rf o

include build/config.mk
include build/rules.mk
include blaze/blaze.mk
include test/test.mk

OBJS	 = $(foreach x,$(PKGS),$($(x)_OBJS))
SRCS	:= $(foreach x,$(PKGS),$($(x)_SRCS))
HDRS	:= $(foreach x,$(PKGS),$($(x)_HDRS))
INCS	 = $(foreach x,$(PKGS),$($(x)_INCS))
BINS	 = $(foreach x,$(PKGS),$($(x)_BINS))
TESTS	 = $(foreach x,$(PKGS),$($(x)_TESTS))
