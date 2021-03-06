#include "stdafx.h"
#include "iostream"
#include "mpi.h"

#define SIZE 10

int main()
{
	int rank, size, bufElems;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 1)
	{
		int arr[SIZE];
		
		//Заполняем массив индексами дл удобства
		for (int i = 0; i < SIZE; i++) arr[i] = i;
		
		// Отправляем этот массив всем потокам кроме самого себя
		for (int i = 0; i < size; i++)
		{
			if (i != rank)
			{
				MPI_Send(arr, SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
			}
		}
	}
	else
	{
		// Проверяем длину входного сообщения
		MPI_Status status;
		//Для гибкости берем сообщения с любых других ресурсов
		MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &bufElems);
		// Выделяем память 
		int *buf = (int*)malloc(sizeof(int) * bufElems);
		// Получаем массив
		MPI_Recv(buf, bufElems, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process: %d = {\t", rank);
		for (int i = 0; i < bufElems; i++)
		{
			printf("%d\t", buf[i]);
		};
		printf("}\n");
	}
	MPI_Finalize();
	return 0;
}