//#include "mpi.h"
//#include <stdlib.h>
//#include <math.h>
//#include <stdio.h>
//#include <time.h>
///* Commands to run:
//   mpiexec -n 10 PP_MPI.exe
//*/
//
//
//
//int range;
//int process_count;
//int procrank;
//double elapsed;
//
//
//int is_prime(int n) {
//    int top, d;
//    top = (int)(sqrt((double)n) + 0.0001);
//    for (d = 2; d <= top; d++)
//        if (n % d == 0)
//            return 0;
//    return 1;
//}
//
//void main(int argc, char** argv) {
//    if(procrank == 0 ){
//        printf("Enter up to what number you want to count the number of primes: ");
//    }
//    scanf("%d", &range);
//    MPI_Init(&argc, &argv);
//    MPI_Comm_rank(MPI_COMM_WORLD, &procrank);
//    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
//    int min;
//    int max;
//    double dt = (double)range / process_count;
//    double start = MPI_Wtime();
//
//    min = (procrank == 0) ? 2 : (int)(procrank * dt) + 1;
//    max = (procrank == process_count - 1) ? range : (int)((procrank + 1) * dt);
//
//    int count = 0;
//
//    int i;
//
//    if (procrank != 0) {
//
//        for (i = min; i <= max; i++)
//            if (is_prime(i))
//                count++;
//        int ds[3];
//        ds[0] = count;
//        ds[1] = min;
//        ds[2] = max;
//        MPI_Send(ds, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
//    }
//    else {
//
//        for (i = min; i <= max; i++)
//            if (is_prime(i))
//                count++;
//
//        int ct;
//        elapsed = MPI_Wtime() - start;
//
//        for (ct = 1; ct < process_count; ct++) {
//
//            int data_received[3];
//            MPI_Status status;
//            MPI_Recv(data_received, 3, MPI_INT, MPI_ANY_SOURCE,
//                MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//            count += data_received[0];
//
//        }
//
//        printf("\nNumber of primes in 1 to %d are \n%d", range, count);
//        printf("\nTime elapsed : %f seconds.\n", elapsed);
//    }
//
//
//    MPI_Finalize();
//
//}
//


#include "mpi.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

int range;
int process_count;
int procrank;
double elapsed;

int is_prime(int n) {
    int top, d;
    top = (int)(sqrt((double)n) + 0.0001);
    for (d = 2; d <= top; d++)
        if (n % d == 0)
            return 0;
    return 1;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &procrank);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    MPI_Barrier(MPI_COMM_WORLD);  // Синхронизация процессов
    
    if (procrank == 0) {
        printf("Enter up to what number you want to count the number of primes: ");
        fflush(stdout);  // Принудительная очистка буфера вывода
        scanf("%d", &range);
    }

    MPI_Bcast(&range, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int min;
    int max;
    double dt = (double)range / process_count;
    double start = MPI_Wtime();

    min = procrank * (range / process_count) + 1;
    max = (procrank + 1) * (range / process_count);

    // Ensure that the last process handles the remaining numbers
    if (procrank == process_count - 1) {
        max = range;
    }

    int count = 0;

    for (int i = min; i <= max; i++) {
        if (is_prime(i)) {
            count++;
        }
    }

    if (procrank != 0) {
        int ds[3];
        ds[0] = count;
        ds[1] = min;
        ds[2] = max;
        MPI_Send(ds, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        int total_count = count;
        elapsed = MPI_Wtime() - start;

        for (int ct = 1; ct < process_count; ct++) {
            int data_received[3];
            MPI_Status status;
            MPI_Recv(data_received, 3, MPI_INT, ct, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            total_count += data_received[0];
        }

        printf("\nNumber of primes in 1 to %d are \n%d", range, total_count-1);
        printf("\nTime elapsed: %f seconds.\n", elapsed);
    }

    MPI_Finalize();

    return 0;
}
