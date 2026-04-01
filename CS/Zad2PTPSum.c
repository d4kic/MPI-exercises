/* zadatak. Napisati program koji nalazi sumu n celih brojeva korišćenjem ptp
komunikacije tako da svaki proces učestvuje u sumiranju */

#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status status;
	int val, sum = 0;
	if (rank == 0)
	{
		val = 1;
		MPI_Send(&val, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
	}
	else
	{
		MPI_Recv(&val, 1, MPI_COUNT, rank - 1, 0, MPI_COMM_WORLD, &status);
		sum = rank + 1 + val;
		if (rank != size - 1)
			MPI_Send(&sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		else
			printf("Process %d has calculated the sum = %d", rank, sum);
	}
	MPI_Finalize();
	return 0;
}