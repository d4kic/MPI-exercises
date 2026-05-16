#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define n 5
#define k 4
#define m 6

int main(int argc, char** argv) {
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int a[n][k], b[k][m], c[n][m];
	int perProc = m / numproc;

	if (rank == 0) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < k; ++j) {
				a[i][j] = i + j + 1;
			}
		}
		for (int i = 0; i < k; ++i) {
			for (int j = 0; j < m; ++j) {
				b[i][j] = i + j + 1;
			}
		}
	}
	MPI_Bcast(a, n * k, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Datatype tmpB, colB;
	MPI_Type_vector(k, perProc, m, MPI_INT, &tmpB);
	MPI_Type_create_resized(tmpB, 0, perProc * sizeof(int), &colB);
	MPI_Type_commit(&colB);

	int* local_b = malloc(k * perProc * sizeof(int));
	// modifikacija umesto scatter
	if (rank == 0) {
		for (int i = 0; i < k; ++i) {
			for (int j = 0; j < perProc; ++j) {
				local_b[i * perProc + j] = b[i][j];
			}
		}
		for (int i = 1; i < numproc; ++i) {
			MPI_Send(&b[0][i * perProc], 1, colB, i, 0, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(local_b, k * perProc, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	int* local_c = malloc(n * perProc * sizeof(int));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < perProc; ++j) {
			local_c[i * perProc + j] = 0;
			for (int l = 0; l < k; ++l) {
				local_c[i * perProc + j] += a[i][l] * local_b[l * perProc + j];
			}
		}
	}

	struct {
		int val;
		int rank;
	} local_max, max;
	local_max.rank = rank;
	local_max.val = INT_MIN;

	for (int i = 0; i < k * perProc; ++i) {
		if (local_b[i] > local_max.val) {
			local_max.val = local_b[i];
		}
	}
	MPI_Reduce(&local_max, &max, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&max, 1, MPI_2INT, 0, MPI_COMM_WORLD);

	MPI_Datatype tmpC, colC;
	MPI_Type_vector(n, perProc, m, MPI_INT, &tmpC);
	MPI_Type_create_resized(tmpC, 0, perProc * sizeof(int), &colC);
	MPI_Type_commit(&colC);

	MPI_Gather(local_c, n * perProc, MPI_INT, c, 1, colC, max.rank, MPI_COMM_WORLD);

	if (rank == max.rank) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				printf("%d ", c[i][j]);
			}
			printf("\n");
		}
	}

	free(local_c);
	free(local_b);
	MPI_Type_free(&colB);
	MPI_Type_free(&colC);
	MPI_Finalize();
	return 0;
}