#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define n 8

int main(int argc, char** argv)
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int perProc = n / numproc;
	if (rank < n % numproc) perProc++;
	int arr[n];
	if (rank == 0)
	{
		printf("Before: ");
		for (int i = 0; i < n; ++i)
		{
			arr[i] = i;
			printf("%d ", arr[i]);
		}
	}
	int* local_arr = (int*)malloc(perProc * sizeof(int));
	MPI_Scatter(arr, perProc, MPI_INT, local_arr, perProc, MPI_INT, 0, MPI_COMM_WORLD);
	for (int i = 0; i < perProc; ++i)
		local_arr[i] += rank;
	MPI_Gather(local_arr, perProc, MPI_INT, arr, perProc, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("\nAfter: ");
		for (int i = 0; i < n; ++i)
			printf("%d ", arr[i]);
	}
	free(local_arr);
	MPI_Finalize();
	return 0;
}
