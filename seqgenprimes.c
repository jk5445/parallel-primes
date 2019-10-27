#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>

int genprimes(int n) {
    bool* a = (bool*) malloc(n * sizeof(bool));
    int i;
    int j;
    for(i = 2; i <= n; i++) {
        a[i-2] = true;
    }
    for(i = 2; i <= (n + 1) / 2; i++) {
	if(!a[i-2])
	    continue;
        for(j = 2 * i; j <= n; j = j + i) {
            a[j-2] = false;
        }
        printf("%d ", i);
    }
    for(; i <= n; i++)
	if(a[i-2])
	    printf("%d ", i);
    printf("\n");
    return 0;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("usage: %s <arg1>\n", argv[0]);
        return 0;
    }
    close(1);
    char* s = malloc(15 * sizeof(char));
    int n = atoi(argv[1]);
    sprintf(s, "results/%d.txt", n);
    open(s, O_CREAT | O_TRUNC | O_RDWR, 0666);
    genprimes(n);
    return 0;
}
