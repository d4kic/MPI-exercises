#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	int rank, p;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	int buf1, buf2;
	if (rank == 0)
	{
		buf1 = 1; buf2 = 2;
		for (int i = 1; i < p; ++i)
		{
			MPI_Send(&buf1, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&buf2, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
		}
		for (int i = 1; i < p; ++i)
		{
			MPI_Recv(&buf1, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Root process %d primio %d\n", rank, buf1);
		}
	}
	else
	{
		MPI_Recv(&buf1, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&buf2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process %d je prvo primio %d pa %d", rank, buf1, buf2);
		MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
