CC=gcc
end=-w -lm -lgsl -lgslcblas -O3

all: stripe2d clean

stripe2d: stripe2d.c calc.o clusters.o data.o list.o loop.o plot.o rng.o run.o save.o set.o step.o
	${CC} -o stripe2d stripe2d.c calc.o clusters.o data.o list.o loop.o plot.o rng.o run.o save.o set.o step.o ${end}

calc.o: calc.c
	${CC} -o calc.o -c calc.c ${end}

clusters.o: clusters.c
	${CC} -o clusters.o -c clusters.c ${end}

data.o: data.c
	${CC} -o data.o -c data.c ${end}

list.o: list.c
	${CC} -o list.o -c list.c ${end}

loop.o: loop.c
	${CC} -o loop.o -c loop.c ${end}

plot.o: plot.c
	${CC} -o plot.o -c plot.c ${end}

rng.o: rng.c
	${CC} -o rng.o -c rng.c ${end}

run.o: run.c
	${CC} -o run.o -c run.c ${end}

save.o: save.c
	${CC} -o save.o -c save.c ${end}

set.o: set.c
	${CC} -o set.o -c set.c ${end}

step.o: step.c
	${CC} -o step.o -c step.c ${end}

clean:
	rm *.o
