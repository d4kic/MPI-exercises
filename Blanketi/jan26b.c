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
	int a[n][n], kolona[n];
	
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

	MPI_Comm diag;
	int color = 0;
	if (red_p == kol_p) {
		color = 1;
	}

	MPI_Comm_split(MPI_COMM_WORLD, color, rank, &diag);

	MPI_Datatype tmp, col;
	MPI_Type_vector(n, 1, n, MPI_INT, &tmp);
	MPI_Type_create_resized(tmp, 0, sizeof(int), &col);
	MPI_Type_commit(&col);

	if (color == 1) {
		int nrank, nnumproc;
		MPI_Comm_rank(diag, &nrank);
		MPI_Comm_size(diag, &nnumproc);
		if (nrank == 0) {
			for (int i = 0; i < n; ++i) {
				kolona[i] = a[i][0];
			}
			for (int i = 1; i < nnumproc; ++i) {
				MPI_Send(&a[0][i], 1, col, i, 0, diag);
			}
		}
		else {
			MPI_Recv(kolona, n, MPI_INT, 0, 0, diag, MPI_STATUS_IGNORE);
		}

		int res = 1;
		for (int i = 0; i < n; ++i) {
			res *= kolona[i];
		}

		printf("Rank %d, proizvod = %d\n", nrank, res);
	}

	MPI_Comm_free(&diag);
	MPI_Type_free(&col);
	MPI_Finalize();
	return 0;
}
