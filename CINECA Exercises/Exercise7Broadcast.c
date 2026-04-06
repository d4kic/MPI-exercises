#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	int rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int val;
	if (rank == 0)
	{
		scanf("%d", &val);
		val = val * val;
	}
	MPI_Bcast(&val, 1, MPI_INT, 0, MPI_COMM_WORLD);
	printf("Process %d got %d", rank, val);
	MPI_Finalize();
	return 0;
}
