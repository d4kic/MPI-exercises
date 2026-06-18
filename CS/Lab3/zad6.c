#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>

#define n 5
#define m 5

// mora da se pokrene sa n*m procesa
int main(int argc, char** argv) {
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int dims[2] = { n, m }, periods[2] = { 0, 0 }, coords[2];

	MPI_Comm cart, col;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart);
	MPI_Cart_coords(cart, rank, 2, coords);

	int color = coords[1];
	MPI_Comm_split(cart, color, rank, &col);

	int nrank, nnumproc;
	MPI_Comm_rank(col, &nrank);
	MPI_Comm_size(col, &nnumproc);

	int id = rank, max_id;
	MPI_Reduce(&id, &max_id, 1, MPI_INT, MPI_MAX, 0, col);

	if (nrank == 0) {
		int coords_max[2];
		MPI_Cart_coords(cart, max_id, 2, coords_max);
		printf("Kolona %d: proces sa najvecim id = %d (%d, %d)\n", coords[1], max_id, coords_max[0], coords_max[1]);
	}

	MPI_Finalize();
	return 0;
}