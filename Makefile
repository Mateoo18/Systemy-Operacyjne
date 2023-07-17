COMPILER = gcc
CFLAGS = -Wall -std=c11 -pedantic

.PHONY: all
all: zad2a.x zad2b.x zad2c.x

.PHONY: run-cw3a
run-cw3a: zad2a.x
	./zad2a.x 

.PHONY: run-cw3b
run-cw3b: zad2b.x zad2a.x
	./zad2b.x 

.PHONY: run-cw3c
run-cw3c: zad2c.x zad2a.x
	./zad2c.x 

.PHONY: cw3a
cw3a: zad2a.x

.PHONY: cw3b
cw3b: zad2b.x zad2a.x

.PHONY: cw3c
cw3c: zad2c.x zad2a.x

%.x : %.c
	 $(COMPILER) $(CFLAGS) -o $@ $<


.PHONY: clean
clean:
	rm *.x
