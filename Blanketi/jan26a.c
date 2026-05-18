#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define n 4
#define s 5
#define k 6

int main(int argc, char** argv) {
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int a[n][s], b[s][k], c[n][k];
	int perProc = k / numproc;
	if (rank == 0) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < s; ++j) {
				a[i][j] = i + j + 1;
			}
		}
		for (int i = 0; i < s; ++i) {
			for (int j = 0; j < k; ++j) {
				b[i][j] = i + j + 1;
			}
		}
	}
	MPI_Bcast(a, n * s, MPI_INT, 0, MPI_COMM_WORLD);

	int* local_b = malloc(s * perProc * sizeof(int));
	MPI_Datatype tmpB, colB;
	MPI_Type_vector(s * perProc, 1, numproc, MPI_INT, &tmpB);
	MPI_Type_create_resized(tmpB, 0, sizeof(int), &colB);
	MPI_Type_commit(&colB);

	MPI_Scatter(b, 1, colB, local_b, s * perProc, MPI_INT, 0, MPI_COMM_WORLD);

	int* local_c = malloc(n * perProc * sizeof(int));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < perProc; ++j) {
			local_c[i * perProc + j] = 0;
			for (int l = 0; l < s; ++l) {
				local_c[i * perProc + j] += a[i][l] * local_b[l * perProc + j];
			}
		}
	}

	int local_mul_b[s], mul_b[s];
	for (int i = 0; i < s; ++i) {
		local_mul_b[i] = 1;
		for (int j = 0; j < perProc; ++j) {
			local_mul_b[i] *= local_b[i * perProc + j];
		}
	}
	MPI_Reduce(&local_mul_b, &mul_b, s, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);

	MPI_Datatype tmpC, colC;
	MPI_Type_vector(n * perProc, 1, numproc, MPI_INT, &tmpC);
	MPI_Type_create_resized(tmpC, 0, sizeof(int), &colC);
	MPI_Type_commit(&colC);

	MPI_Gather(local_c, n * perProc, MPI_INT, c, 1, colC, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < k; j++) {
				printf("%d ", c[i][j]);
			}
			printf("\n");
		}
		printf("Proizvodi vrsta matrice B: ");
		for (int i = 0; i < s; ++i) {
			printf("%d ", mul_b[i]);
		}
	}

	free(local_c);
	free(local_b);
	MPI_Type_free(&colB);
	MPI_Type_free(&colC);
	MPI_Finalize();
	return 0;
}
