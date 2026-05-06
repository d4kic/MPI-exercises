#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define n 8

int main(int argc, char** argv)
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int perProc = n / numproc;
	int a[n], b[n];
	int* local_a = malloc(perProc * sizeof(int));
	int* local_b = malloc(perProc * sizeof(int));
	MPI_Datatype tmp, elements;
	MPI_Type_vector(perProc, 1, numproc, MPI_INT, &tmp);
	MPI_Type_create_resized(tmp, 0, sizeof(int), &elements);
	MPI_Type_commit(&elements);

	if (rank == 0)
	{
		for (int i = 0; i < n; ++i)
		{
			a[i] = i + 1;
			b[i] = i + 1;
		}
	}
	MPI_Scatter(a, 1, elements, local_a, perProc, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(b, 1, elements, local_b, perProc, MPI_INT, 0, MPI_COMM_WORLD);

	int local_res = 0, res;
	for (int i = 0; i < perProc; ++i)
	{
		local_res += local_a[i] * local_b[i];
	}
	MPI_Reduce(&local_res, &res, 1, MPI_INT, MPI_SUM, 1, MPI_COMM_WORLD);

	if (rank == 1)
	{
		printf("Skalarni proizvod je %d\n", res);
	}

	MPI_Type_free(&elements);
	free(local_a);
	free(local_b);
	MPI_Finalize();
	return 0;
}