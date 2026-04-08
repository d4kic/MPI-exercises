#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <limits.h>

#define n 6

int main(int argc, char** argv)	
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int perProc = n / numproc;
	double minval = INT_MAX;
	double* local_niz = (double*)malloc(perProc * sizeof(double));
	if (rank == 0)
	{
		double niz[n];
		for (int i = 0; i < n; ++i)
			niz[i] = i + 1;
		int offset = perProc;
		for (int i = 1; i < numproc; ++i)
		{
			MPI_Send(niz + offset, perProc, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
			offset += perProc;
		}
		for (int i = 0; i < perProc; ++i)
			local_niz[i] = niz[i];
	}
	else
	{
		MPI_Recv(local_niz, perProc, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	for (int i = 0; i < perProc; ++i)
		minval = minval > local_niz[i] ? local_niz[i] : minval;

	if (rank == 0)
	{
		double min = minval;
		for (int i = 1; i < numproc; ++i)
		{
			double local_min;
			MPI_Recv(&local_min, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			min = min > local_min ? local_min : min;
		}
		printf("Min je %.2lf", min);
	}
	else
	{
		MPI_Send(&minval, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
