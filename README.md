# Parallel Primes

## MPI

This project uses MPI. MPI is a library for parallel processes. The processes do not share memory and can only communicate through MPI communication calls.
While more complex algorithms and/or a shared memory approach would yield a better speedup, the goal of this project is to increase familiarity with MPI and the distributed memory approach to parallelization.

## Sieve of Eratosthenes

The sequential code implements the [Sieve of Eratoshenes](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes).

![Sieve In Action](https://upload.wikimedia.org/wikipedia/commons/b/b9/Sieve_of_Eratosthenes_animation.gif)

Note the small deviation and optimization from the traditional Sieve of Eratosthenes: for each prime factor *p* the code steps throught the sieve with steps of size *p* rather than checking every element for divisibility by *p*.

The parallel version splits the crossing out of elements in the sieve among the processors. To limit costly communication between processors, each process:
1. generates all primes less than the square root of n sequentially
2. marks all multiples of the generated prime numbers in the process's block of the sieve
3. sends the marked block to the master process

## Performance

The performance of the parallelized version is significantly worse for problem sizes smaller than 10 million.
This is mostly due to MPI overhead. MPI initialization alone takes a few hundred milliseconds.

As the problem size increases we do see good speed up, but the speedup is limited by the need to write the primes sequentially.

## Runinng The Code

To run the parallelized code you will need MPI [openmpi](https://www.open-mpi.org)

*mpicc -Wall -std=c99 -o genprimes genprimes.c* to compile the parralelized code and *mpiexec -n [x] ./genprimes [m]* to generate all primes up to *m* with *x* processes in parallel.

*gcc -Wall -std=c99 -o seqgenprimes seqgenprimes.c* to compile the sequential code and *./seqgenprimes [m]* to generate all primes up to *m* sequentially.

The results will be written to results/*m*.txt.

To check the correctness of the results for m < 15,000,000 run test.py with *-n=m* as an argument: *python3 test.py -n=[m]*
