#include "stdafx.h"
#include <stdio.h>
#include <mpi.h>

#define N 4
template <int rows, int cols>
void print(int(&matrix)[rows][cols])
{
	printf("\n");
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

int main()
{
	int i, j, k, rank, size;
	int a[N][N];
	int b[N][N];
	for (int p1 = 0; p1 < N; p1++)
	{
		for (int p2 = 0; p2 < N; p2++)
		{
			a[p1][p2] = p1 * p2;
			b[p1][p2] = p1 * p2;
		}
	}


	
	int c[N][N];
	int aa[N], bb[N], cc[N];

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Scatter(a, N * N / size, MPI_INT, aa, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(b, N * N / size, MPI_INT, bb, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);

	for (i = 0; i < N; i++)
		cc[i] = aa[i] * bb[i];

	MPI_Gather(cc, N * N / size, MPI_INT, c, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0)
		print(c);
	MPI_Finalize();
}

