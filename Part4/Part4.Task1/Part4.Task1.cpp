#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define N 5

int main()
{
	MPI_Init(NULL, NULL);
	int i, rank, size, global;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	srand(time(NULL));

	if (size > N)
	{
		printf("Proccess count must be less as %d", N);
		MPI_Finalize();
		return 1;
	}

	int baseArr[N][N];
	if (rank == 0)
	{
		for (i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)	
			{
				baseArr[j][i] = i * i * i+ j;
				printf("%d\t", baseArr[j][i]);
			}
			printf("\n");
		}
	}
	int *vectorArr = (int *) (malloc(sizeof(int) * N * N / size));

	MPI_Scatter(baseArr, N * N / size, MPI_INT, vectorArr, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);

	int sum = 0;
	for (i = 0; i < N; i++)
		sum += vectorArr[i];

	MPI_Reduce(&sum, &global, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Finalize();

	if (rank == 0)
		printf("%d", global);
}
