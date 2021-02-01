# Target library
lib := libuthread.a
objs := context.o preempt.o queue.o uthread.o
uth := uthread.o
CC := gcc
CFLAGS := -Wall -Wextra -Werror
all: $(lib)

deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

%.o : %.c
	@echo "CC $@"
	@$(CC) $(CFLAGS) -c -o $@ $< $(DEPFLAGS)

$(lib) : $(objs)
	ar rcs $(lib) $^	

clean: 
	rm -f $(lib) $(objs) $(deps)
  
