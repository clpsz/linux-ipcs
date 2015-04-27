CFLAGS = -Wall
LDFLAGS = -lpthread

target = _main
ifeq ($(MAKECMDGOALS), good)
srcs = main_good.c debug.c
else
srcs = main.c debug.c
endif
objs = $(srcs:.c=.o)
headers = $(wildcard *.h)


.PHONY: all
all: $(target)

$(target): $(objs) $(headers) FORCE
	$(CC) -o $@ $(objs) $(LDFLAGS)
	@-rm -f *.o

good: all

$(objs):%.o:%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: FORCE
FORCE: 

clean:
	rm -f $(target) *.o

