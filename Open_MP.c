
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>

int prime = 2;

int is_prime(int n) {
    int s = sqrt(n);
    for (int y = 3; y <= s; y += 1) {
        if (!(n % y)) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char** argv) {
    struct timeval tval_before, tval_after, tval_result;
    printf("Enter num: ");
    int fin;
    scanf("%d", &fin);

    // Изменения для управления числом процессов
    int num_threads = 1; // По умолчанию 1 процесс
    if (argc > 1) {
        num_threads = atoi(argv[1]); // Если передано значение через командную строку
    }
    omp_set_num_threads(num_threads);

    gettimeofday(&tval_before, NULL);

    #pragma omp parallel for
    for (int x = 5; x <= fin; x += 2) {
        if (is_prime(x)) {
            #pragma omp critical
            {
                prime += 1;
            }
        }
    }

    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    printf("\nNumber of primes in 1 to %d are \n%d\n", fin, prime);
    printf("\nTime elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

    return 0;
}
