#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main()
{

	MPI_Init(NULL, NULL);
	int rank, size;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	const int N = 20;
	int partSize = N / size - 1;

	int arr[N];
	int* receive_vector = (int *)malloc(sizeof(int) *partSize);

	srand(time(NULL));

	if (rank == 0)
	{
		for (int i = 0; i < N; i++)
		{
			arr[i] = rand();
		}
	}

	MPI_Scatter(arr, partSize, MPI_INT, receive_vector, partSize, MPI_INT, 0, MPI_COMM_WORLD);
	int local[2], global[2];

	printf("Process %d = {\t ", rank);
	for (int i = 0; i < partSize; i++)
	{
		printf("%d\t", receive_vector[i]);
	}
	printf("}\n");

	MPI_Barrier(MPI_COMM_WORLD);

	local[0] = receive_vector[0];
	for (int i = 1; i < partSize; i++)
	{
		if (receive_vector[i] < local[0])
		{
			local[0] = receive_vector[i];
		}
		local[1] = rank;
	}

	MPI_Reduce(&local, &global, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		int index = 0;
		for (int i = 0; i < N; i++)
		{
			if (arr[i] == global[0])
			{
				index = i;
				break;
			}
		}
		printf("Process %d: array[%d] = %d\n", global[1], index, global[0]);
	}

	MPI_Finalize();

	return 0;
}