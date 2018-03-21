# fns - Factorial Number System

According to [Wikipedia](https://en.wikipedia.org/wiki/Factorial_number_system), "the factorial number system,
also called factoradic, is a mixed radix numeral system adapted to numbering permutations". This repository aims
at offering a series of algorithms to perform basic arithmetic operations between two numbers in this number system,
in the form of a simple interactive calculator (see [code](https://github.com/lluisalemanypuig/fns/blob/master/src/calculator.cpp)).

It also offers a simple code to process all permutations of a list of N elements in parellel (here using OpenMP)
(see [code](https://github.com/lluisalemanypuig/fns/blob/master/src/permutations.cpp)). The code will process all
permutations of a list of N elements only when the number of threads used is a divisor of the number of permutations.
For small values of N (N <= 7) this code is not needed since there are not too many permutations (7! = 5040). For
all larger values of N (N >= 8), 2^6 = 64 threads is a divisor of N!. Using a divisor of N! as the number of threads
is also useful to balance the load among the threads equally (in terms of the amount of permutations processed).
Although it's not likely to be needed, this software also allows arbitrarily large values of N.

## Dependencies

The [GMP](https://gmplib.org/) library is needed to compile this project. Also, the [OpenMP](http://www.openmp.org/)
library is needed for parallelisation (when processing the permutations in parallel).

## Compilation and execution

### Compilation

There are two modes of compilation: debug and release. Issue the following commands for each mode:

#### Debug

$ make -f build-rules/Makefile debug

#### Release

$ make -f build-rules/Makefile release

### Execution

Two executable files will be generated after all the code has been compiled. The calculator has an interactive mode and
another that allows the loading and execution of a program.

In order to use the interactive mode, issue the command:

$ ./calculator -i

If the other mode is preferred then issue the following command:

$ ./calculator -l /path/to/dir/file_name

where /path/to/dir/file_name contains the path (absolute or relative) and the name to the file that is to be executed.

If one wants to run the demo to process the permutations of a list in parallel issue the following command:

$ ./process_permutations N t

where N is the length of the list of elements to be permuted, and t is the number threads. For example, in order to
process all permutations of a list of N=10 elements with t=8 threads issue the following command:

$ ./process_permutations 10 8
