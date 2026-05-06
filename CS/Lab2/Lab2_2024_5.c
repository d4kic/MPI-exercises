#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define n 12

int main(int argc, char** argv)
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int perProc = n / numproc;
	int a[n];
	int* local_a = malloc(perProc * sizeof(int));

	if (rank == 0)
	{
		for (int i = 0; i < n; ++i)
			a[i] = i + 1;
	}
	MPI_Scatter(a, perProc, MPI_INT, local_a, perProc, MPI_INT, 0, MPI_COMM_WORLD);
	int local_sum = 0;
	for (int i = 0; i < perProc; ++i)
		local_sum += local_a[i];
	int sum;
	MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	double avg;
	if (rank == 0)
		avg = (double)sum / n;
	MPI_Bcast(&avg, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	double local_expr = 0;
	for (int i = 0; i < perProc; ++i)
		local_expr += avg + local_a[i];
	double expr;
	MPI_Reduce(&local_expr, &expr, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	double b, c;
	if (rank == numproc - 1)
	{
		b = 2.0;
		c = 3.0;
	}

	MPI_Bcast(&b, 1, MPI_DOUBLE, numproc - 1, MPI_COMM_WORLD);
	MPI_Bcast(&c, 1, MPI_DOUBLE, numproc - 1, MPI_COMM_WORLD);

	if (rank == 0)
	{
		double res = expr / (b + c);
		printf("Rezultat = %f\n", res);
	}

	free(local_a);
	MPI_Finalize();
	return 0;
}