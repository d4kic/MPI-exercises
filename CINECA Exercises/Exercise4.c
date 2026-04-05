#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int a = rank, b, sum = 0;
	int right = (rank + 1) % numproc;
	int left = int left = rank == 0 ? numproc - 1 : rank - 1;
	MPI_Status status;
	MPI_Request req;
	for (int i = 0; i < numproc; ++i)
	{
		MPI_Isend(&a, 1, MPI_INT, left, 0, MPI_COMM_WORLD, &req);
		MPI_Recv(&b, 1, MPI_INT, right, 0, MPI_COMM_WORLD, &status);
		MPI_Wait(&req, &status);
		a = b;
		sum += a;
	}
	printf("Process %d has sum = %d", rank, sum);
	fflush(stdout);
	MPI_Finalize();
	return 0;
}