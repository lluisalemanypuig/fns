# fns - Factorial Number System

According to [Wikipedia](https://en.wikipedia.org/wiki/Factorial_number_system), "the factorial number system,
also called factoradic, is a mixed radix numeral system adapted to numbering permutations". This repository aims
at offering a series of algorithms to perform basic arithmetic operations between two numbers in this number system,
in the form of a simple interactive calculator (see [code](https://github.com/lluisalemanypuig/fns/blob/master/src/calculator.cpp)). This calculator supports a series of commands, listed in the section "Commands available for
the calculator" of this document.

It also offers a simple code to process all permutations of a list of N elements in parellel (here using OpenMP)
(see [code](https://github.com/lluisalemanypuig/fns/blob/master/src/permutations.cpp)). The code processes all
permutations of a list of N elements only when the number of threads used is a divisor of the number of permutations.
If it is not then some permutations will be left unprocessed.
For small values of N (N <= 7) this code is not needed since there are not too many permutations (7! = 5040). For
all larger values of N (N >= 8), 2^6 = 64 threads is a divisor of N!.
Although it is not likely to be needed, this software also allows arbitrarily large values of N.

## Dependencies

### Libraries

The [GMP](https://gmplib.org/) and the [OpenMP](http://www.openmp.org/) libraries are needed to compile this project.
OpenMp library is only needed to generate the executable that processes permutations in parallel.

### Tools

Compiling requires the 'make' tool, and a compiler (g++) that supports the flag -std=c++11.

## Compilation and execution

### Compilation

There are two modes of compilation: debug and release. These two modes will create, respectively, a _bin-debug/_
and a _bin-release/_ directories. From within the _build/_ directory, issue the following commands for each mode:

#### Debug

    make -f Makefile debug

#### Release

    make -f Makefile release

### Execution

Two executable files will be generated after all the code has been compiled. The calculator has an interactive mode and
another that allows the loading and execution of a program.

In order to use the interactive mode, issue the command:

    ./calculator -i

The list of all the commands supported can be obtained by entering the "help" command (without the quotes in the
interactive mode of the calculator).

If the other mode is preferred then issue the following command:

    ./calculator -l /path/to/dir/file_name

where /path/to/dir/file_name contains the path (absolute or relative) and the name to the file that is to be executed.

If one wants to run the demo to process the permutations of a list in parallel issue the following command:

    ./process_permutations N t

where N is the length of the list of elements to be permuted, and t is the number threads. For example, in order to
process all permutations of a list of N=10 elements with t=8 threads issue the following command:

    ./process_permutations 10 8

#### Commands available for the calculator

These are all the commands available for the calculator (this list may not be up-to-date with the actual commands
supported - see the result of "help" in the interactive mode of the calculator for a full and up-to-date description
of all of them):

    > var v s: stores in memory a variable with name 'v' and contents
          's', where 's' is a number in base 10.
    > op s1 X s2: operates contents of s1 and s2 with operator X.
          The operators supported are +,-,*
          s1 and s2 must be either a variable name or a number in base 10
    > def varname s1 X s2: creates a variable with name 'varname'
          with contents the result of operating the contents of s1
          and s2 with operator X.
          s1 and s2 must be either a variable name or a number in base 10
    > cmp s1 C s2: compares the contents of s1 and s2 with C.
          The comparisons supported are >,>=,==,<=,<
          s1 and s2 must be either a variable name or a number in base 10

    > inc v: apply ad-hoc algorithm to increment by 1 the contents of v.
          v must be a variable name
    > dec v: apply ad-hoc algorithm to decrement by 1 the contents of v.
          v must be a variable name
    > double v: apply ad-hoc algorithm to double the contents of v.
          v must be a variable name
    > halve v: apply ad-hoc algorithm to halve the contents of v.
          v must be a variable name
    > square v: apply ad-hoc algorithm to square of the contents of v.
          v must be a variable name
    > even v: checks if the contents of v is even.
          v must be a variable name

    > factorial v n: compute the factorial of n and store in v.
          v must be a variable name
          n must be a number in base 10
    > make-permutation i n s_1 s_2 ... s_n: computes the i-th permutation of
          the list of 'n' elements (s_1,...,s_n).
          i must be either a variable name or a number in base 10
          n must be a number in base 10 that fits in a 64-bit number
    > permutation-index n s_1 s_2 ... s_n a_1 a_2 ... a_n: computes the index of
          the permutation (s_1,...,s_n) of the list (a_1,...,a_n).
          The index is given as a number in base 10.
          n must be a number in base 10 that fits in a 64-bit number

    > repeat n OPTION: repeats the given COMMAND n times.
          n must be a number in base 10
    > { COMMAND_1 COMMAND_2 ... COMMAND_N }: allows defining a block of instructions.
          This can be useful to repeat a list of options in block
    > // ... //: this is a comment. The contents of '...' may be arbitrarily long.

    > shrink: removes all leading zeros from all variables in order to save space
    > shrink-var: removes all leading zeros from all variables in order to save space
    > del-var v: delete variable with name 'v'
    > del: removes all all variables

    > verbose: activate/deactivate printing execution times
    > endlines: activate/deactivate printing leading and trailing endlines

    > ls: lists all variables and their content (in factorial base)
    > ls-dec: lists all variables and their content (in base 10)
    > print v: prints the contets of variable v (in factorial base)
    > print-dec v: prints the contents of v (in base 10)

    > exit: leave the program ('CTRL + D' is also valid)
    > help: print this message
