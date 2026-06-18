#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define n 5

int main(int argc, char** argv) {
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);

	const int v = 250;
	MPI_Comm novi;
	int color = (rank % 3 == 0) ? 1 : 0;
	MPI_Comm_split(MPI_COMM_WORLD, color, rank, &novi);

	if (color == 1) {
		int nrank, nnumproc;
		MPI_Comm_rank(novi, &nrank);
		MPI_Comm_size(novi, &nnumproc);

		int* row = (int*)malloc(n * sizeof(int));
		if (nrank == 0) {
			int** a = (int**)malloc(nnumproc * sizeof(int*));
			for (int i = 0; i < nnumproc; ++i) {
				a[i] = (int*)malloc(n * sizeof(int));
			}

			for (int i = 0; i < nnumproc; ++i) {
				for (int j = 0; j < n; ++j) {
					a[i][j] = i + j + 1;
				}
			}

			for (int i = 1; i < nnumproc; ++i) {
				MPI_Send(a[i], n, MPI_INT, i, 0, novi);
			}

			for (int i = 0; i < n; ++i) {
				row[i] = a[0][i];
			}

			// dealloc
			for (int i = 0; i < nnumproc; ++i) {
				free(a[i]);
			}
			free(a);
		}
		else {
			MPI_Recv(row, n, MPI_INT, 0, 0, novi, MPI_STATUS_IGNORE);
		}

		int prod = 1;
		for (int i = 0; i < n; ++i) {
			prod *= row[i];
		}
		if (prod <= v) {
			printf("Proizvod %d je manji od v = %d, proces %d (stari rank %d).\n", prod, v, nrank, rank);
		}

		free(row);
	}

	MPI_Finalize();
	return 0;
}
