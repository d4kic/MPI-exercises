#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

// TWO PROCESSES ONLY!
int main(int argc, char** argv)
{
	srand(time(0));
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int local_value;
	if (rank == 0)
	{
		local_value = rand() % 5 + 1;
		int recv;
		MPI_Send(&local_value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		MPI_Recv(&recv, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process %d got %d, sum of values is %d", rank, recv, local_value + recv);
	}
	else
	{
		local_value = rand() % 10 + 5;
		int recv;
		MPI_Recv(&recv, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Send(&local_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		printf("Process %d got %d, product of values is %d", rank, recv, local_value * recv);
	}
	MPI_Finalize();
	return 0;
}