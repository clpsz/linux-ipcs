CFLAGS = -Wall
LDFLAGS = -lpthread

target1 = multi_threads_sem
target2 = multi_processes_sem

.PHONY: all
all: $(target1) $(target2)

$(target1): thread_sem.c debug.c
	$(CC) -o $@ $^ $(LDFLAGS)
	@-rm -f *.o

$(target2): process_sem.c debug.c
	$(CC) -o $@ $^ $(LDFLAGS)
	@-rm -f *.o

$(objs):%.o:%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: FORCE
FORCE: 

clean:
	rm -f $(target1) $(target2) *.o

