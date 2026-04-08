#include <stdio.h>
#include <mpi.h>

#define len 20

int main(int argc, char** argv)	
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	double start = MPI_Wtime();
	if (rank == 0)
	{
		char msg1[len] = "poruka1", msg2[len] = "poruka2";
		for (int i = 1; i < numproc; ++i)
		{
			MPI_Send(msg1, len, MPI_CHAR, i, 0, MPI_COMM_WORLD);
			MPI_Send(msg2, len, MPI_CHAR, i, 1, MPI_COMM_WORLD);
		}
		for (int i = 1; i < numproc; ++i)
		{
			int recv;
			MPI_Recv(&recv, 1, MPI_INT, i, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Process 0 primio %d\n", recv);
		}
	}
	else
	{
		char msg1[len], msg2[len];
		MPI_Recv(&msg1, len, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&msg2, len, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process %d primio %s i %s", rank, msg1, msg2);
		MPI_Send(&rank, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
	}
	double end = MPI_Wtime();
	if (rank == 0)
		printf("Time: %f", end - start);
	MPI_Finalize();
	return 0;
}
