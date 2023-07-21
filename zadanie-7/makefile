COMPILER = gcc
CFLAGS = -Wall -std=c99 -pedantic
LFLAGS = -pthread -lrt
DIR = $(CURDIR)

vpath %.c %.h %.a %.o lib
BUILDDIR = lib

.PHONY: all
all: glowny.x producent.x konsument.x glowny.e producent.e konsument.e glowny.d producent.d konsument.d


.PHONY: run-cw7static
run-cw7static: glowny.x producent.x konsument.x
	./glowny.x ./surowce ./skonsumowane ./producent.x ./konsument.x read_lock write_lock /shared_mem
	diff -s ./surowce ./skonsumowane


.PHONY: run-cw7shared1
run-cw7shared1: glowny.d producent.d konsument.d
	./glowny.d ./surowce ./skonsumowane ./producent.d ./konsument.d read_lockx write_lockx /shared_memx
	diff -s ./surowce ./skonsumowane

.PHONY: run-cw7shared2
run-cw7shared2: glowny.e producent.e konsument.e
	export LD_LIBRARY_PATH=${DIR}/lib; \
	./glowny.e ./surowce ./skomsumowane ./producent.e ./konsument.e read_lockc write_lockc /shared_memc
	diff -s ./surowce ./skonsumowane



.PHONY: cw7static
cw7static: glowny.x producent.x konsument.x

.PHONY: cw7shared1
cw7shared1: glowny.d producent.d konsument.d

.PHONY: cw7shared2
cw7shared2: glowny.e producent.e konsument.e


%.x : %.c lib/all.a
	 $(COMPILER) $(CFLAGS) -o $@ $? $(LFLAGS)


%.o : %.c %.h
	$(COMPILER) $(CFLAGS) -c $< $(LFLAGS)


/lib/all.so : semafor.o pamdziel.o
	$(COMPILER) -shared -o lib/liball.so $? $(LFLAGS)

lib/all.a : semafor.o pamdziel.o
	ar -cr $@ $?


%.d : %.c /lib/all.so
	$(COMPILER) -L$(DIR)/lib $(CFLAGS)  -o $@ $< -lall -Wl,--rpath=$(DIR)/lib

%.e : %.c /lib/all.so
	$(COMPILER) -L$(DIR)/lib  $(CFLAGS)  -o $@ $< -lall


.PHONY: clean
clean:
	
	rm *.x
	rm *.o
	rm lib/*.so
	rm lib/*.a
	rm *.d
	rm *.e
