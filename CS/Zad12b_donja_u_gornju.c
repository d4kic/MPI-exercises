/* zad. Napisati program kojim se elementi gornje trougaone matrice Anxn
procesa 0, salju u gornju trougaonu matricu Tnxn procesa 1.
(varijanta donja trougaona u gornju) */

#include <stdio.h>
#include <mpi.h>

#define n 5

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	int rank, numproc;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int displacements[n], block_lengths[n];
	for (int i = 0; i < n; ++i) {
		block_lengths[i] = i + 1;
		displacements[i] = i * n;
	}
	MPI_Datatype vrste_donja, vrste_gornja;
	MPI_Type_indexed(n, block_lengths, displacements, MPI_INT, &vrste_donja);
	MPI_Type_commit(&vrste_donja);
	for (int i = 0; i < n; ++i) {
		block_lengths[i] = n - i;
		displacements[i] = (n + 1) * i;
	}
	MPI_Type_indexed(n, block_lengths, displacements, MPI_INT, &vrste_gornja);
	MPI_Type_commit(&vrste_gornja);
	if (rank == 0) {
		int a[n][n];
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				a[i][j] = i + j + 1;
			}
		}
		MPI_Send(a, 1, vrste_donja, 1, 0, MPI_COMM_WORLD);
	}
	else {
		int t[n][n];
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				t[i][j] = 0;
			}
		}
		MPI_Recv(t, 1, vrste_gornja, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				printf("%d ", t[i][j]);
			}
			printf("\n");
		}
	}
	MPI_Type_free(&vrste_donja);
	MPI_Type_free(&vrste_gornja);
	MPI_Finalize();
	return 0;
}