#include <stdio.h>
#include <mpi.h>

#define n 6

int main(int argc, char** argv)
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int bafer[n], sum = 0, uslov;
	for (int i = 0; i < n; ++i)
		bafer[i] = rank + i;
	for (int i = 0; i < n; ++i)
		sum += bafer[i];

	if (rank != 0)
	{
		uslov = sum % rank == 0 ? 1 : 0;
		MPI_Send(&uslov, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else
	{
		for (int i = 1; i < numproc; ++i)
		{
			MPI_Recv(&uslov, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (uslov)
				printf("Proces %d ispunjava uslov\n", i);
		}
	}
	MPI_Finalize();
	return 0;
}
