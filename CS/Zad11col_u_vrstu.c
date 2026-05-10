/* zad. Napisati program kojim se koriscenjem izvedenih tipova podataka vrsi
slanje kolone 0 procesa 0 u vrstu 0 procesa 1. */

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define n 5

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	int rank, numproc;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int a[n][n];
	MPI_Datatype tmp, col;
	MPI_Type_vector(n, 1, n, MPI_INT, &tmp);
	MPI_Type_create_resized(tmp, 0, sizeof(int), &col);
	MPI_Type_commit(&col);
	if (rank == 0) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				a[i][j] = i + j;
			}
		}
		MPI_Send(&a[0][0], 1, col, 1, 0, MPI_COMM_WORLD);
	}
	else {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				a[i][j] = 0;
			}
		}
		MPI_Recv(&a[0][0], n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				printf("%d ", a[i][j]);
			}
			printf("\n");
		}
	}
	MPI_Finalize();
	return 0;
}