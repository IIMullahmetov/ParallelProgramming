#include "stdafx.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAG_SEND_A 1
#define TAG_SEND_B 2
#define TAG_SEND_C 3
#define SIZE_MATRIX 8

int main()
{
	srand(time(NULL));
	MPI_Init(NULL, NULL);
	int processors_count;
	MPI_Comm_size(MPI_COMM_WORLD, &processors_count);
	int processor_number;
	MPI_Comm_rank(MPI_COMM_WORLD, &processor_number);
	int size_send = SIZE_MATRIX / (processors_count - 1);
	int a[SIZE_MATRIX][SIZE_MATRIX], b[SIZE_MATRIX][SIZE_MATRIX];
	int c[SIZE_MATRIX][SIZE_MATRIX];

	if (processor_number == 0)
	{
		//Отправка векторов
		for (int i = 0; i < SIZE_MATRIX; i++)
		{
			for (int j = 0; j < SIZE_MATRIX; j++)
			{
				a[i][j] = 0 + rand() % 10;
				b[i][j] = 0 + rand() % 10;
			}
		}
		printf("ARRAY A: \n");
		for (int i = 0; i < SIZE_MATRIX; i++)
		{
			for (int j = 0; j < SIZE_MATRIX; j++)
			{
				printf("%d  ", a[i][j]);
			}
			printf("\n");
		}
		printf("\n");
		printf("ARRAY B: \n");
		for (int i = 0; i < SIZE_MATRIX; i++)
		{
			for (int j = 0; j < SIZE_MATRIX; j++)
			{
				printf("%d  ", b[i][j]);
			}
			printf("\n");
		}
		printf("\n");

		int begin = 0;
		for (int i = 1; i < processors_count; i++)
		{

			MPI_Send(&a[begin], size_send*SIZE_MATRIX, MPI_INT, i, TAG_SEND_A,
				MPI_COMM_WORLD);
			MPI_Send(&b, SIZE_MATRIX*SIZE_MATRIX, MPI_INT, i, TAG_SEND_B,
				MPI_COMM_WORLD);
			begin += size_send;
		}
		printf("\nSEND FROM %d\n", processor_number);

		//Обработка результатов
		begin = 0;
		for (int i = 1; i < processors_count; i++)
		{
			MPI_Recv(&c[begin], size_send*SIZE_MATRIX, MPI_INT, i, TAG_SEND_C, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			begin += size_send;
		}

		printf("Result:\n");
		for (int i = 0; i < SIZE_MATRIX; i++)
		{
			for (int j = 0; j < SIZE_MATRIX; j++)
			{
				printf("%d  ", c[i][j]);
			}
			printf("\n");
		}
	}
	else
	{
		MPI_Status status;
		int count_data;

		int **recv_a = (int **)malloc(SIZE_MATRIX * sizeof(int*));
		int **total_c = (int **)malloc(SIZE_MATRIX * sizeof(int*));
		//int recv_a[size_send][SIZE_MATRIX], total_c[size_send][SIZE_MATRIX];
		//        MPI_Probe(0, TAG, MPI_COMM_WORLD, &status);
		MPI_Recv(recv_a[0], size_send*SIZE_MATRIX, MPI_INT, 0, TAG_SEND_A, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(b[0], SIZE_MATRIX*SIZE_MATRIX, MPI_INT, 0, TAG_SEND_B, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		for (int i = 0; i < size_send; i++)
		{
			for (int j = 0; j < SIZE_MATRIX; j++)
			{
				total_c[i][j] = 0;
			}
		}
		for (int i = 0; i < size_send; i++)
		{
			for (int j = 0; j < SIZE_MATRIX; j++)
			{
				for (int k = 0; k < SIZE_MATRIX; k++)
				{
					total_c[i][j] += recv_a[i][k] * b[k][j];
				}
			}
		}
		MPI_Send(total_c, size_send*SIZE_MATRIX, MPI_INT, 0, TAG_SEND_C, MPI_COMM_WORLD);


	}
	MPI_Finalize();
}

