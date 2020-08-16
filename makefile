gcc_options = -std=c++17 -Wall --pedantic-errors

program: main.cpp all.h
	clang++ $(gcc_options) -o $@ $<

array_test: array_test.cpp array.hpp all.h
	clang++ $(gcc_options) -o $@ $<

all.h.gch: all.h
	clang++ $(gcc_options) -x c++-header -o $@ $<

array.hpp.gch: array.hpp
	clang++ $(gcc_options) -x c++-header -o $@ $<

run : program
	./program

clean:
	rm -f ./program
	rm -f ./all.h.gch
	rm -f ./array_test

.PHONY: run clean