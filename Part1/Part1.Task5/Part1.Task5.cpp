#include "stdafx.h"
#include "iostream"
#include "mpi.h"

#define SIZE 12

int main()
{
	int rank, size, bufElems;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0)
	{
		int arr[SIZE];
		for (int i = 0; i < SIZE; i++) arr[i] = i;


		printf("Process: %d = {\t", rank);

		for (int j = 0; j < SIZE / (size + 1); j++)
		{
			printf("%d\t", arr[j]);
		}
		printf("}\n");

		for (int i = 1; i < size; i++)
		{
			int index = 0;
			int *subarr = (int *)malloc(sizeof(int) * SIZE / (size + 1));
			for (int j = i * SIZE / (size + 1); j < (i + 1) * SIZE / (size + 1); j++)
			{
				subarr[index] = arr[j];
				index++;
			}
			MPI_Send(subarr, SIZE / (size + 1), MPI_INT, i, 0, MPI_COMM_WORLD);
		};

		//TODO исправить этот кусок говна которое никак не робит
		/*MPI_Status status;
		MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &bufElems);
		int *buf = (int *)malloc(sizeof(int) * bufElems);
		MPI_Recv(buf, bufElems, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < bufElems; i++)
		{
			printf("%d ", buf[i]);
		};
		printf("\nНомер процесса: %d\n", rank);*/
		//free(buf);
	}
	else
	{
		MPI_Status status;
		MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &bufElems);
		int *buf = (int *)malloc(sizeof(int) * bufElems);
		MPI_Recv(buf, bufElems, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process: %d = {\t", rank);
		for (int i = 0; i < bufElems; i++)
		{
			printf("%d\t", buf[i]);
		};
		printf("}\n");
		free(buf);
	}
	MPI_Finalize();
	return 0;
}