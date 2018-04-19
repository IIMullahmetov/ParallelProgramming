#include "stdafx.h"
#include "iostream"
#include "mpi.h"
#include "vector"

using namespace std;

#define SIZE 10

void printVector(vector<int> x)
{
	for (int i = 0; i < x.size(); i++)
	{
		printf("%d ", x[i]);
	};
	printf("\n");
}

int main()
{
	int rank, size;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int part = SIZE / (size - 1);
	if (rank == 0)
	{
		vector<int> x(SIZE), y(SIZE);
		for (int i = 0; i < SIZE; i++)
		{
			x[i] = i;
			y[i] = 1;
		}
		printVector(x);
		printVector(y);

		for (int i = 1; i < size; i++)
		{
			vector<int> xPart(&x[part * (i - 1)], &x[part * i]),
				yPart(&y[part * (i - 1)], &y[part * i]), result(part), auxResult(part);

			MPI_Send(&xPart[0], xPart.size(), MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&yPart[0], yPart.size(), MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Status status1;
			MPI_Status status2;
			MPI_Recv(&result[0], part, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status1);
			MPI_Recv(&auxResult[0], part, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &status2);
			printf("From process %d - x: ", status1.MPI_SOURCE);
			printVector(result);

			printf("From process %d - y: ", status2.MPI_SOURCE);
			printVector(auxResult);
		}
	}
	else
	{
		vector<int> xPart(part), yPart(part), result(part);

		MPI_Recv(&xPart[0], part, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&yPart[0], part, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		std::swap(xPart, yPart);
		MPI_Send(&xPart[0], result.size(), MPI_INT, 0, 2, MPI_COMM_WORLD);
		MPI_Send(&yPart[0], result.size(), MPI_INT, 0, 3, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}