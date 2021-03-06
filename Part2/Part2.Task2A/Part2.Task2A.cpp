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
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int size_send = (SIZE_MATRIX*SIZE_MATRIX) / (processors_count - 1);
	int a[SIZE_MATRIX][SIZE_MATRIX], b[SIZE_MATRIX][SIZE_MATRIX], c[SIZE_MATRIX][SIZE_MATRIX];

	if (rank == 0)
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
		printf("ARRAY B:\n");
		for (int i = 0; i < SIZE_MATRIX; i++)
		{
			for (int j = 0; j < SIZE_MATRIX; j++)
			{
				printf("%d  ", b[i][j]);
			}
			printf("\n");
		}
		printf("\n");

		int rows = 0;
		int column = 0;
		for (int i = 1; i < processors_count; i++)
		{
			int *send_a = (int *)malloc(sizeof(int) * size_send);
			int *send_b = (int *)malloc(sizeof(int) * size_send);;
			rows = (size_send*(i - 1)) / SIZE_MATRIX;
			column = (size_send*(i - 1)) % SIZE_MATRIX;
			for (int j = 0; j < size_send; j++) 
			{
				send_a[j] = a[rows][column];
				send_b[j] = b[rows][column];
				column++;
				if (column == SIZE_MATRIX)
				{
					rows++;
					column = 0;
				}
			}
			MPI_Send(send_a, size_send, MPI_INT, i, TAG_SEND_A, MPI_COMM_WORLD);
			MPI_Send(send_b, size_send, MPI_INT, i, TAG_SEND_B, MPI_COMM_WORLD);
			column += size_send;
			while (column >= SIZE_MATRIX) 
			{
				rows++;
				column -= size_send;
			}
		}
		printf("Result:\n");
		for (int i = 1; i < processors_count; i++) 
		{
			rows = (size_send*(i - 1)) / SIZE_MATRIX;
			column = (size_send*(i - 1)) % SIZE_MATRIX;
			MPI_Recv(&c[rows][column], size_send, MPI_INT, i, TAG_SEND_C, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		
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
		int *recv_a = (int *)malloc(sizeof(int) * size_send);
		int *recv_b = (int *)malloc(sizeof(int) * size_send);
		int *recv_c = (int *)malloc(sizeof(int) * size_send);
		MPI_Recv(recv_a, size_send, MPI_INT, 0, TAG_SEND_A, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(recv_b, size_send, MPI_INT, 0, TAG_SEND_B, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process: %d\n", rank);
		for (int i = 0; i < size_send; i++)
		{
			recv_c[i] = recv_a[i] * recv_b[i];
			printf("%d  ", recv_c[i]);
		}
		printf("\n");
		MPI_Send(recv_c, size_send, MPI_INT, 0, TAG_SEND_C, MPI_COMM_WORLD);

	}
	// Finalize the MPI environment.
	MPI_Finalize();
}