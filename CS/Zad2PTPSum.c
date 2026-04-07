/* zadatak. Napisati program koji nalazi sumu n celih brojeva korišćenjem ptp
komunikacije tako da svaki proces učestvuje u sumiranju */

#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int val;
	if (rank == 0)
	{
		val = 1;
		MPI_Send(&val, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
	}
	else
	{
		MPI_Recv(&val, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		int sum = val + rank + 1;
		if (rank != numproc - 1)
			MPI_Send(&sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		else
			printf("Suma je %d", sum);
	}
	MPI_Finalize();
	return 0;
}