#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define m 6
#define k 6

int main(int argc, char** argv) {
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int a[m][k], b[k], c[m];
	int perProc = m / numproc;
	
	if (rank == 0) {
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < k; ++j) {
				a[i][j] = i + j + 1;
			}
		}
		for (int i = 0; i < k; ++i) {
			b[i] = 1;
		}
	}
	MPI_Bcast(b, k, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Datatype tmpA, rowA;
	MPI_Type_vector(perProc, k, k * numproc, MPI_INT, &tmpA);
	MPI_Type_create_resized(tmpA, 0, k * sizeof(int), &rowA);
	MPI_Type_commit(&rowA);

	int* local_a = malloc(k * perProc * sizeof(int));
	MPI_Scatter(a, 1, rowA, local_a, k * perProc, MPI_INT, 0, MPI_COMM_WORLD);

	int* local_c = malloc(perProc * sizeof(int));
	for (int i = 0; i < perProc; ++i) {
		local_c[i] = 0;
		for (int j = 0; j < k; ++j) {
			local_c[i] += local_a[i * k + j] * b[j];
		}
	}
	
	MPI_Datatype tmpC, typeC;
	MPI_Type_vector(perProc, 1, numproc, MPI_INT, &tmpC);
	MPI_Type_create_resized(tmpC, 0, sizeof(int), &typeC);
	MPI_Type_commit(&typeC);

	MPI_Gather(local_c, perProc, MPI_INT, c, 1, typeC, 0, MPI_COMM_WORLD);

	MPI_Finalize();
	return 0;
}
