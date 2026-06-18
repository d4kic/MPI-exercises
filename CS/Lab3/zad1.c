#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define n 5

int main(int argc, char** argv) {
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);

	const int v = 10;
	MPI_Comm deljivi;
	int color = (rank % 6 == 0) ? 1 : 0;
	MPI_Comm_split(MPI_COMM_WORLD, color, rank, &deljivi);

	if (color == 1) {
		int nrank, nnumproc;
		MPI_Comm_rank(deljivi, &nrank);
		MPI_Comm_size(deljivi, &nnumproc);

		int* col = (int*)malloc(n * sizeof(int));
		if (nrank == 0) {
			// matrica, ima nnumproc redova jer se svakom procesu salje po jedna
			int** a = (int**)malloc(n * sizeof(int*));
			for (int i = 0; i < n; ++i) {
				a[i] = (int*)malloc(nnumproc * sizeof(int));
			}

			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < nnumproc; ++j) {
					a[i][j] = i + j + 1;
				}
			}

			int* buff = (int*)malloc(n * sizeof(int));
			for (int j = 1; j < nnumproc; ++j) {
				for (int i = 0; i < n; ++i) {
					buff[i] = a[i][j];
				}
				MPI_Send(buff, n, MPI_INT, j, 0, deljivi);
			}

			// lokalno za proc0
			for (int i = 0; i < n; ++i) {
				col[i] = a[i][0];
			}

			// dealloc
			for (int i = 0; i < n; ++i) {
				free(a[i]);
			}	
			free(a);
			free(buff);
		}
		else {
			MPI_Recv(col, n, MPI_INT, 0, 0, deljivi, MPI_STATUS_IGNORE);
		}

		int sum = 0;
		for (int i = 0; i < n; ++i) {
			sum += col[i];
		}

		if (sum >= v) {
			printf("Suma %d je veca od v = %d, proces %d (stari rank %d).\n", sum, v, nrank, rank);
		}

		free(col);
	}

	MPI_Finalize();
	return 0;
}
