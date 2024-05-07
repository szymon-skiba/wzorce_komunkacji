MPICXX = mpicxx
CFLAGS = -lm

SOURCES = nwd.cpp wzorzec_potokowy.cpp
EXECUTABLE = a.out

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(MPICXX) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES)

run:
	mpirun --oversubscribe -np 8 ./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
