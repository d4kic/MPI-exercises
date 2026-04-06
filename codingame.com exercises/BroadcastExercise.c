#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int n, *buff;
	if (rank == 0)
	{
		scanf("%d", &n);
		buff = (int*)malloc(n * sizeof(int));
		for (int i = 0; i < n; ++i)
			scanf("%d", &buff[i]);
	}
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank != 0) buff = (int*)malloc(n * sizeof(int));
	MPI_Bcast(buff, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	int sum = 0;
	if (rank == 0)
	{
		for (int i = 0; i < n; ++i)
			sum += buff[i];
	}
	if (rank == 1)
	{
		for (int i = 0; i < n; ++i)
			if (buff[i] % 2 == 0)
				sum += buff[i];
	}
	if (rank == 2)
	{
		for (int i = 0; i < n; ++i)
			if (buff[i] % 2 != 0)
				sum += buff[i];
	}
	printf("Process %d has sum %d", rank, sum);
	free(buff);
	MPI_Finalize();
	return 0;
}
