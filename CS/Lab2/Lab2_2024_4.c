#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define k 6

struct minbuf
{
	int val;
	int rank;
};

int main(int argc, char** argv)
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int a[k][k];
	int redovi = k / 2;
	int perProc = redovi / numproc;
	int* local_a = malloc(perProc * k * sizeof(int));
	MPI_Datatype rows, resized;
	MPI_Type_vector(redovi, k, k * 2, MPI_INT, &rows);
	MPI_Type_create_resized(rows, 0, perProc * k * sizeof(int), &resized);
	MPI_Type_commit(&resized);

	if (rank == 2)
	{
		int val = 100;
		for (int i = 0; i < k; ++i)
			for (int j = 0; j < k; ++j)
				a[i][j] = val--;
	}
	MPI_Scatter(a, 1, resized, local_a, redovi * k, MPI_INT, 2, MPI_COMM_WORLD);

	int local_min = local_a[0];
	for (int i = 0; i < perProc * k; ++i)
	{
		if (local_a[i] < local_min)
			local_min = local_a[i];
	}

	struct minbuf local_pair, g_pair;
	local_pair.val = local_min;
	local_pair.rank = rank;

	MPI_Reduce(&local_pair, &g_pair, 1, MPI_2INT, MPI_MINLOC, 2, MPI_COMM_WORLD);
	if (rank == 2)
	{
		printf("Minimum je %d, rank je %d\n", g_pair.val, g_pair.rank);
	}

	MPI_Type_free(&resized);
	free(local_a);
	MPI_Finalize();
	return 0;
}