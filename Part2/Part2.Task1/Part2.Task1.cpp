#include "stdafx.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#define TAG_SEND_X 1
#define TAG_SEND_Y 2
#define TAG_SEND_Z 3
#define SIZE_VECTOR 8

int main() 
{
	MPI_Init(NULL, NULL);
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int processor_number;
	MPI_Comm_rank(MPI_COMM_WORLD, &processor_number);
	int a = 14, b = 8;
	int size_send = SIZE_VECTOR / (size - 1);
	int z[SIZE_VECTOR];

	if (processor_number == 0) 
	{
		//Отправка векторов
		int  x[SIZE_VECTOR] = { 0,1,2,3,4,5,6,7 };
		int y[SIZE_VECTOR] = { 10, 12, 14, 7, 4 ,3,2,1 };
		printf("Vector x:");
		for (int i = 0; i < SIZE_VECTOR; i++) 
		{
			printf(" %d ", x[i]);
		}
		printf("\n Vector y:");

		for (int i = 0; i < SIZE_VECTOR; i++)
		{
			printf("%d ", y[i]);
		}
		int begin = 0;
		for (int i = 1; i < size; i++) {
			MPI_Send(&x[begin], size_send, MPI_INT, i, TAG_SEND_X,
				MPI_COMM_WORLD);
			MPI_Send(&y[begin], size_send, MPI_INT, i, TAG_SEND_Y,
				MPI_COMM_WORLD);
			begin += size_send;
		}
		printf("\nSEND FROM %d\n", processor_number);

		//Обработка результатов
		begin = 0;
		for (int i = 1; i < size; i++) 
		{
			MPI_Recv(&x[begin], size_send, MPI_INT, i, TAG_SEND_X, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&y[begin], size_send, MPI_INT, i, TAG_SEND_Y, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&z[begin], size_send, MPI_INT, i, TAG_SEND_Z, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			begin += size_send;
		}
		for (int i = 0; i < SIZE_VECTOR; i++)
		{
			printf("Vector x:");
			printf("%d ", x[i]);
		}
		printf("\n Vector y:");
		for (int i = 0; i < SIZE_VECTOR; i++) 
		{
			printf("%d ", y[i]);
		}
		printf("\nVector z:");
		for (int i = 0; i < SIZE_VECTOR; i++)
		{
			printf("%d ", z[i]);
		}

	}
	else {
		MPI_Status status;
		int count_data;
		int *recv_x = (int *)malloc(sizeof(int) * size_send);
		int *recv_y = (int *)malloc(sizeof(int) * size_send);
		//        MPI_Probe(0, TAG, MPI_COMM_WORLD, &status);
		MPI_Recv(recv_x, size_send, MPI_INT, 0, TAG_SEND_X, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(recv_y, size_send, MPI_INT, 0, TAG_SEND_Y, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		printf("Hello from processor number %d"
			" out of %d processors\n", processor_number, size);

		for (int i = 0; i < size_send; i++) {
			z[i] = a * recv_x[i] + b * recv_y[i];
		}
		MPI_Send(z, size_send, MPI_INT, 0, TAG_SEND_Z, MPI_COMM_WORLD);

		for (int i = 0; i < size_send; i++) {
			int tmp = recv_x[i];
			recv_x[i] = recv_y[i];
			recv_y[i] = tmp;
		}
		MPI_Send(recv_x, size_send, MPI_INT, 0, TAG_SEND_X, MPI_COMM_WORLD);
		MPI_Send(recv_y, size_send, MPI_INT, 0, TAG_SEND_Y, MPI_COMM_WORLD);


	}
	// Finalize the MPI environment.
	MPI_Finalize();
}