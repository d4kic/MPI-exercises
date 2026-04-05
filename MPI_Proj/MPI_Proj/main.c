#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define n 6

int main(int argc, char** argv)
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int a[n][n], b[n];
	if (rank == 0)
	{
		for (int i = 0; i < n; ++i)
		{
			b[i] = 1;
			for (int j = 0; j < n; ++j)
				a[i][j] = i + j;
		}
	}
	int local_a[n], local_res = 0, res[n];
	MPI_Scatter(a, n, MPI_INT, local_a, n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(b, n, MPI_INT, 0, MPI_COMM_WORLD);
	for (int i = 0; i < n; ++i)
		local_res += local_a[i] * b[i];
	MPI_Gather(&local_res, 1, MPI_INT, res, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0)
		for (int i = 0; i < n; ++i)
			printf("%d ", res[i]);
	MPI_Finalize();
	return 0;
}
