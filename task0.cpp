/*
 * Task zero Course MPI MIPT 2019
 * by Timofey Chulkov
 * group: 797
 * */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> //заголовочный файл mpi
#include <math.h>
#include <string.h>


int main(int argc, char *argv[]) {
    int i = 0, sum = 0;
    int N = 1000000000;
    int myrank, size;

    MPI_Status Status; // тип данных mpi

    /*
     * MPI программа начинается с MPI_Init; все остальные N процессова начинают работу
     * после этого
     * */
    MPI_Init(&argc, &argv);

    /*
     * переменной size присвается число, равное кол-ву процессов
     * */

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /*
     * каждому процессу присваевается уникальный номер
     * */
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    int size_of_row = floor(N/(size - 1)) + 1;

    // массив размера sizeXfloor(N/(size - 1)) + 1



    int array[size_of_row];
    memset(array, 0, size_of_row*sizeof(int));

    if(myrank == 0){

        for(int row = 0; row < size - 1; row++) {

            for(int index = 0; index < size_of_row && i < N; i++ && index ++){
                array[index] = i;
            }

            // отсылаем порцию информации на узлы
            MPI_Send(&array[0], size_of_row, MPI_INT, row + 1, 1, MPI_COMM_WORLD);

            memset(array, 0, size_of_row*sizeof(int));
        }

    } else {
        MPI_Recv(&array, size_of_row, MPI_INT, 0, 1, MPI_COMM_WORLD, &Status);
        int sum_temporary = 0;


        printf("Procces: %d : items \n", myrank);

        for(int index = 0; index < size_of_row; index++) {
            sum_temporary += array[index];
            printf("%d,  ", array[index]);
        }

        printf("\nProcces: %d sum is %d \n", myrank, sum_temporary);

        MPI_Send(&sum_temporary, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }

    if (myrank == 0) {
        int temporary_sum = 0;
        for(int index = 0; index < size - 1; index++) {
            MPI_Recv(&temporary_sum, 1, MPI_INT, index+1, 2, MPI_COMM_WORLD, &Status);

            sum += temporary_sum;
        }

        printf("Paralles sum is %d\n", sum);
        printf("Classic sum is %d\n", N*(N-1)/2);


    }
    /*
     * MPI программа должна закончить с использованием функцией MPI Finalize
     * */
    MPI_Finalize();

    return 0;

}
