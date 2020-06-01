gcc_options = -std=c++17 -Wall --pedantic-errors

program: main.cpp all.h
	clang++ $(gcc_options) -include all.h -o $@ $<

all.h.gch: all.h
	clang++ $(gcc_options) -x c++-header -o $@ $<

run : program
	./program

clean:
	rm -f ./program
	rm -f ./all.h.gch

.PHONY: run clean