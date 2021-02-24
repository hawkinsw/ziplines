all: ziplines.cpp Makefile
	g++ -g -o ziplines ziplines.cpp

test: all
	./test.sh

clean:
	rm -f *.out core ziplines
