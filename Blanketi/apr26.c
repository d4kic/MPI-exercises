#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define k 4
#define m 9

int main(int argc, char** argv) {
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int a[k][m], b[m], c[k];
	int perProc = m / numproc;

	/*
	FILE* f = fopen("april23.dat", "r");
	for (int i=0; i<k; ++i)
		for (int j=0; j<m; ++j)
			fscanf(f, "%d", &a[i][j]);
	fclose(f);
	*/

	if (rank == 0) {
		for (int i = 0; i < k; ++i) {
			for (int j = 0; j < m; ++j) {
				a[i][j] = i + j + 1;
			}
		}
		for (int i = 0; i < m; ++i) {
			b[i] = 1;
		}
	}
	MPI_Bcast(b, m, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Datatype tmpA, colA;
	MPI_Type_vector(k, perProc, m, MPI_INT, &tmpA);
	MPI_Type_create_resized(tmpA, 0, perProc * sizeof(int), &colA);
	MPI_Type_commit(&colA);

	int* local_a = malloc(k * perProc * sizeof(int));
	MPI_Scatter(a, 1, colA, local_a, k * perProc, MPI_INT, 0, MPI_COMM_WORLD);

	int local_c[k];
	for (int i = 0; i < k; ++i) {
		local_c[i] = 0;
		for (int j = 0; j < perProc; ++j) {
			local_c[i] += local_a[i * perProc + j] * b[rank * perProc + j];
		}
	}

	int root = numproc - 1;
	MPI_Reduce(local_c, c, k, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
	if (rank == root) {
		for (int i = 0; i < k; ++i) {
			printf("%d ", c[i]);
		}
	}

	MPI_Finalize();
	return 0;
}
