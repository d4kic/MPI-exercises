#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define k 4

int main(int argc, char** argv)
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int a[k][k], b[k][k], local_a[k];
	MPI_Datatype col, resizedCol;
	MPI_Type_vector(k, 1, k, MPI_INT, &col);
	MPI_Type_create_resized(col, 0, sizeof(int), &resizedCol);
	MPI_Type_commit(&resizedCol);
	
	if (rank == 0)
	{
		int val = 1;
		for (int i = 0; i < k; ++i)
		{
			for (int j = 0; j < k; ++j)
			{
				a[i][j] = val;
				b[i][j] = val;
				val++;
			}
		}
	}
	MPI_Bcast(b, k * k, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(a, 1, resizedCol, local_a, k, MPI_INT, 0, MPI_COMM_WORLD);
	int local_c[k], c[k][k];
	for (int i = 0; i < k; ++i)
	{
		local_c[i] = 0;
		for (int j = 0; j < k; ++j)
		{
			local_c[i] += b[i][j] * local_a[j];
		}
	}
	MPI_Gather(local_c, k, MPI_INT, c, 1, resizedCol, numproc - 1, MPI_COMM_WORLD);
	if (rank == numproc - 1)
	{
		for (int i = 0; i < k; ++i)
		{
			for (int j = 0; j < k; ++j)
			{
				printf("%d ", c[i][j]);
			}
			printf("\n");
		}
	}
	MPI_Type_free(&resizedCol);
	MPI_Finalize();
	return 0;
}