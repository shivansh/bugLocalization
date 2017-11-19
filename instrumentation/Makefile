# NOTE: '~/rose_install' should be updated with
# path to the local Rose installation directory.
CXX      = $(shell ~/rose_install/bin/rose-config cxx)
CPPFLAGS = $(shell ~/rose_install/bin/rose-config cppflags)
CXXFLAGS = $(shell ~/rose_install/bin/rose-config cxxflags)
LDFLAGS  = $(shell ~/rose_install/bin/rose-config ldflags)
LIBDIRS  = $(shell ~/rose_install/bin/rose-config libdirs)

all: gen_line_num count_true_branch

gen_line_num.o: gen_line_num.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $^
gen_line_num: gen_line_num.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

count_true_branch.o: count_true_branch.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $^
count_true_branch: count_true_branch.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean run generate_correlations

clean:
	rm -f gen_line_num gen_line_num.o \
	      count_true_branch count_true_branch.o \
	      a.out rose_tcas.c

generate_correlations:
	Rscript correlations.R

run:
	./process_csv.sh testcases/testcase testcases/tcas.c
