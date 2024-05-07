MPICXX = mpicxx
CFLAGS = -lm

SOURCES = nwd.cpp wzorzec_upowszechnianie.cpp
EXECUTABLE = a.out

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(MPICXX) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES)

# Uruchamianie programu z opcją oversubscribe
run:
	mpirun --oversubscribe -np 8 ./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
