#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <limits.h>

#define n 8

int main(int argc, char** argv)
{
	int rank, p;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	int perProc = n / p;
	double buff[n];
	double* local_buf = (double*)malloc(perProc * sizeof(double));
	if (rank == 0)
	{
		for (int i = 0; i < n; ++i)
			scanf("%lf", &buff[i]);
	}
	MPI_Scatter(buff, perProc, MPI_DOUBLE, local_buf, perProc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	double min, local_min = INT_MAX;
	for (int i = 0; i < perProc; ++i)
	{
		if (local_min > local_buf[i])
			local_min = local_buf[i];
	}
	MPI_Reduce(&local_min, &min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
	if (rank == 0) printf("Min is %.2lf", min);

	free(local_buf);
	MPI_Finalize();
	return 0;
}
