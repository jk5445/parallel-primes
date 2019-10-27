#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>


void seqgenprimes(int* primes, int n);
void eliminate(char* sieve, int* primes, int arr_sz, int diff); 

int main(int argc, char* argv[]) {
    int comm_sz;
    int my_rank;
    int arr_sz;
    // difference between the ith index of a process's sieve
    // and the prime number it represents
    int diff;
    int i;
    int n;
    // adjusted n to be divisible by comm_sz
    int n_new;
    int sqrtn;

    int* primes;
    char* sieve;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if(argc != 2) {
        if(my_rank == 0)
            printf("usage: %s <arg1>\n", argv[0]);
        MPI_Finalize();
        return 0;
    }

    n = atoi(argv[1]);
    n_new = n + comm_sz - n % comm_sz;

    arr_sz = n_new / comm_sz;
    diff = my_rank * arr_sz;
    sieve = (char*) calloc(arr_sz, sizeof(char));

    sqrtn = ceil(sqrt((double) n));
    primes = (int*) calloc(sqrtn, sizeof(int));
    seqgenprimes(primes, sqrtn);

    eliminate(sieve, primes, arr_sz, diff);

    if(my_rank == 0) {
        //change standard out to N.txt
        close(1);
        char* s = malloc(15 * sizeof(char));
        sprintf(s, "results%d.txt", n);
        open(s, O_CREAT | O_TRUNC | O_RDWR, 0666);
	
        //gather results
        char* results = malloc(n_new * sizeof(char));
        MPI_Gather(sieve, arr_sz, MPI_CHAR, results, arr_sz, MPI_CHAR, 0, MPI_COMM_WORLD);

        for(i = 2; i <= n; i++) {
            if(!results[i])
                printf("%d ", i);
        }
        printf("\n");
    } else {
        MPI_Gather(sieve, arr_sz, MPI_CHAR, NULL, arr_sz, MPI_CHAR, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}

// generate the primes less than n sequentially
// n here should be the square root of the original problem size
void seqgenprimes(int* primes, int n) {
    int i;
    int j;
    int count = 0;
    int sqrtn = ceil(sqrt((double) n));

    for(i = 2; i <= sqrtn; i++) {
        if(primes[i-2])
            continue;
        for(j = i * i; j <= n; j = j + i) {
            primes[j-2] = 1;
        }
        primes[count++] = i;
    }
    for(; i <= n; i++)
        if(!primes[i-2])
            primes[count++] = i;
    primes[count] = 0;
}

// cross out non primes in process's section of the sieve
void eliminate(char* sieve, int* primes, int arr_sz, int diff) {
    int i;
    int p;
    int sq;
    int j = 0;
    while((p = primes[j++])) {
        sq = p * p;
        //set i to smallest multiple of p that should be crossed
        i = (sq >= diff) ? sq - diff : 0 - diff % p;
        if(i < 0)
            i = i + p;
        for(; i < arr_sz; i = i + p)
                sieve[i] = 1;
    }
}
