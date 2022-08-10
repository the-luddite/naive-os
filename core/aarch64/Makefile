__C = $(wildcard *.c)
__S := $(wildcard *.S)
OBJECTS = $(__C:.c=.o) $(__S:.S=.o)

all:
	@make make_s
	@make make_c

make_c: $(OBJECTS)
	$(CC) $(CFLAGS) -c $< -o $@ 

make_s: $(OBJECTS)
	$(CC) -Iinclude -c $< -o $@ 

clean:
	rm -r *.o

.PHONY: clean