#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define n 4

int main(int argc, char** argv) {
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int a[n][n], diag[n];

	if (rank == 0) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				a[i][j] = i + j + 1;
			}
		}
	}

	int q = (int)sqrt(numproc);
	int red_p = rank / q;
	int kol_p = rank % q;

	int color = 0;
	if (red_p >= kol_p) {
		color = 1;
	}

	MPI_Comm novi;
	MPI_Comm_split(MPI_COMM_WORLD, color, rank, &novi);

	MPI_Datatype tmp, dType;
	MPI_Type_vector(n, 1, n + 1, MPI_INT, &tmp);
	MPI_Type_create_resized(tmp, 0, sizeof(int), &dType);
	MPI_Type_commit(&dType);

	if (color == 1) {
		int nrank, nnumproc;
		MPI_Comm_rank(MPI_COMM_WORLD, &nrank);
		MPI_Comm_size(MPI_COMM_WORLD, &nnumproc);

		if (nrank == 0) {
			for (int i = 0; i < n; ++i) {
				diag[i] = a[i][i];
			}
			for (int i = 1; i < nnumproc; ++i) {
				MPI_Send(&a[0][0], 1, dType, i, 0, novi);
			}
		}
		else {
			MPI_Recv(diag, n, MPI_INT, 0, 0, novi, MPI_STATUS_IGNORE);
		}
	}

	MPI_Comm_free(&novi);
	MPI_Type_free(&dType);
	MPI_Finalize();
	return 0;
}
