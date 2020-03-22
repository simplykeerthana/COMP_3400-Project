# 
# On cs340.cs.uwindsor.ca GCC version 10 (alpha release) has been installed 
# so some C++20 code code can be compiled. 
#

# Define test width and height...
WIDTH=100
HEIGHT=100

# Define C++20 compiler settings...
CXX20=g++-10.0.1
CXX20FLAGS=-std=c++2a -Wall -Wextra -Wold-style-cast -fconcepts -O3 -DPARALLEL

# Define C++17 compiler (must support C++ Parallel Algorithms!) settings...
CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -Wold-style-cast -O3
CXXFLAGS_SEQ=$(CXXFLAGS) -DSEQUENTIAL
CXXFLAGS_PAR=$(CXXFLAGS) -DPARALLEL

# Libraries to explicitly link to...
LIBFLAGS_SEQ=
LIBFLAGS_PAR=-pthread -ltbb

all: \
	main-seq.exe \
	main-par.exe \
	test-ppm-seq.exe \
	test-hsv-seq.exe \
	test-ppm_pixel-seq.exe

clean:
	rm -vf *.exe *.o *.out *.deps mandelbrot*.ppm mandelbrot*.png

# Rule for C++20 concept demo...
cxx20-demo:
	$(CXX20) $(CXX20FLAGS) -o concept-demo.exe concept-demo.cxx $(LIBFLAGS_PAR)
	@rm -f concept-demo.exe
	@echo "Concept checks passed."

test: testseq testpar

testseq: main-seq.exe
	rm -vf mandelbrot-seq.ppm mandelbrot-seq.png
	./$< -w $(WIDTH) -h $(HEIGHT) -o mandelbrot-seq.ppm
	pnmtopng <mandelbrot-seq.ppm >mandelbrot-seq.png

testpar: main-par.exe
	rm -vf mandelbrot-par.ppm mandelbrot-par.png
	./$< -w $(WIDTH) -h $(HEIGHT) -o mandelbrot-par.ppm
	pnmtopng <mandelbrot-par.ppm >mandelbrot-par.png

check: main-seq.exe main-par.exe valid-800x800-mandelbrot.ppm
	rm -f mandelbrot-seq.ppm mandelbrot-seq.png
	rm -f mandelbrot-par.ppm mandelbrot-par.png
	./main-seq.exe -w 800 -h 800 -o mandelbrot-seq.ppm >/dev/null 2>/dev/null
	./main-par.exe -w 800 -h 800 -o mandelbrot-par.ppm >/dev/null 2>/dev/null
	@(cmp -s valid-800x800-mandelbrot.ppm mandelbrot-seq.ppm && echo "Sequential output passed.") || echo "Sequential outuput failed."
	@(cmp -s valid-800x800-mandelbrot.ppm mandelbrot-par.ppm && echo "Parallel output passed.") || echo "Parallel output failed."

gencheckfile: main-seq.exe main-par.exe
	rm -f mandelbrot-seq.ppm mandelbrot-seq.png
	rm -f mandelbrot-par.ppm mandelbrot-par.png
	./main-seq.exe -w 800 -h 800 -o valid-800x800-mandelbrot-seq.ppm >/dev/null 2>/dev/null
	./main-par.exe -w 800 -h 800 -o valid-800x800-mandelbrot-par.ppm >/dev/null 2>/dev/null
	rm -f valid-800x800-mandelbrot-par.ppm
	mv valid-800x800-mandelbrot-seq.ppm valid-800x800-mandelbrot.ppm

-include *.deps

#
# Rules to build sequential code...
#
%-seq.deps: %.cxx
	@set -e ; \
	rm -f $@ ; \
	$(CXX) -MM $(CXXFLAGS_SEQ) $< >$@.$$$$ $(LIBFLAGS_SEQ); \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' <$@.$$$$ >$@ ;\
	rm -f $@.$$$$

%-seq.o: %.cxx %-seq.deps
	$(CXX) $(CXXFLAGS_SEQ) -c -o $@ $< $(LIBFLAGS_SEQ)

%-seq.exe: %-seq.o %-seq.deps
	$(CXX) $(CXXFLAGS_SEQ) -o $@ $< $(LIBFLAGS_SEQ)
	strip -d ./$@

#
# Rules to build parallel code...
#
%-par.deps: %.cxx
	@set -e ; \
	rm -f $@ ; \
	$(CXX) -MM $(CXXFLAGS_PAR) $< >$@.$$$$ $(LIBFLAGS_PAR) ; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' <$@.$$$$ >$@ ;\
	rm -f $@.$$$$

%-par.o: %.cxx %-par.deps
	$(CXX) $(CXXFLAGS_PAR) -c -o $@ $< $(LIBFLAGS_PAR)

%-par.exe: %-par.o %-par.deps
	$(CXX) $(CXXFLAGS_PAR) -o $@ $< $(LIBFLAGS_PAR)
	strip -d ./$@

