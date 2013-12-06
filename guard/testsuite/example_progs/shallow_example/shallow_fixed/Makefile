CC = cc
CFLAGS = -g -h noc99
LIB =

CFILES = calc.c copy.c diag.c init.c main.c time.c tstep.c calc2.c copy2.c diag2.c init2.c main2.c time2.c tstep2.c dump.c
OBJECTS = calc.o copy.o diag.o dump.o init.o time.o tstep.o main.o
OBJECTS2 = calc2.o copy2.o diag2.o dump.o init2.o time2.o tstep2.o main2.o
HEADERS = decs.h

all: pshallow cshallow pshallow2

cshallow:
	$(CC) -g -o cshallow cshallow.c -lm

pshallow: $(OBJECTS) $(HEADERS)
	$(CC) -g -o pshallow $(OBJECTS) -lm $(LIB)

pshallow2: $(OBJECTS2) $(HEADERS)
	$(CC) -g -o pshallow2 $(OBJECTS2) -lm $(LIB)

tags: $(SOURCES) $(HEADERS)
	ctags -w $(SOURCES) $(HEADERS)

clean:
	-rm -f cshallow pshallow pshallow2 $(OBJECTS) $(OBJECTS2) core

calc.o: calc.c decs.h 
copy.o: copy.c decs.h 
diag.o: diag.c decs.h 
init.o: init.c decs.h 
main.o: main.c decs.h 
time.o: time.c decs.h 
tstep.o: tstep.c decs.h

main2.o: main2.c decs.h
diag2.o: diag2.c decs.h
time2.o: time2.c decs.h
tstep2.o: tstep2.c decs.h
calc2.o: calc2.c decs.h
init2.o: init2.c decs.h
copy2.o: copy2.c decs.h

dump.o: dump.c decs.h

cshallow: cshallow.c
