.phony: clean all

all: can.o can_test.o
	$(CC) -o can_test $^
%.o: %.c
	$(CC) -c -o $@ $<

clean:
	rm -rf can_test *.o

