#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

// Структура для передачи параметров потоку
struct ThreadData {
    int start;
    int end;
    int *result;
};

// Функция для проверки, является ли число простым
int isPrime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

// Функция, которую будет выполнять каждый поток
void *countPrimes(void *param) {
    struct ThreadData *data = (struct ThreadData *)param;
    int count = 0;
    
    for (int i = data->start; i <= data->end; i++) {
        if (isPrime(i)) {
            count++;
        }
    }

    *(data->result) = count;

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <количество_потоков>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int numThreads = atoi(argv[1]);

    int targetNumber;
    printf("Введите число, до которого нужно посчитать простые числа: ");
    scanf("%d", &targetNumber);

    // Измерение времени начала выполнения программы
    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    pthread_t threads[numThreads];
    struct ThreadData threadData[numThreads];
    int results[numThreads];

    int numbersPerThread = targetNumber / numThreads;

    for (int i = 0; i < numThreads; i++) {
        threadData[i].start = i * numbersPerThread + 1;
        threadData[i].end = (i == numThreads - 1) ? targetNumber : (i + 1) * numbersPerThread;
        threadData[i].result = &(results[i]);

        pthread_create(&threads[i], NULL, countPrimes, (void *)&threadData[i]);
    }

    int totalCount = 0;
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
        totalCount += results[i];
    }

    // Измерение времени окончания выполнения программы
    clock_gettime(CLOCK_MONOTONIC, &finish);

    // Вычисление и вывод времени выполнения
    double elapsed = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1e9;
    printf("Количество простых чисел до %d: %d\n", targetNumber, totalCount);
    printf("Время выполнения: %.6f сек\n", elapsed);

    return EXIT_SUCCESS;
}
