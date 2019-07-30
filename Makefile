CC =$(CROSS_COMPILER)gcc
.phony: clean
can: can.c
	$(CC) can.c -o can -Wall
clean:
	rm -rf can
