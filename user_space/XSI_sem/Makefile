CFLAGS = -Wall
LDFLAGS = 
ifeq ($(MAKECMDGOALS), good)
CFLAGS += -DUSING_SEM
endif

target = _main
srcs = main.c debug.c
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

