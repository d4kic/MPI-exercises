#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define m 5
#define n 5

// mora da se pokrene sa m*n procesa
int main(int argc, char** argv) {
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int dims[2] = { m, n }, periods[2], color, coords[2];
	int coords_l[2], coords_d[2];
	int levi, desni;

	periods[0] = 0;
	periods[1] = 1;

	MPI_Comm cart;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart);
	MPI_Cart_coords(cart, rank, 2, coords);
	MPI_Cart_shift(cart, 1, 3, &levi, &desni);

	printf("Proces %d sa koordinatama [%d, %d]:\n", rank, coords[0], coords[1]);
	if (levi != -1) {
		MPI_Cart_coords(cart, levi, 2, coords_l);
		printf("Levi sused proces %d sa koordinatama [%d, %d]\n", levi, coords_l[0], coords_l[1]);
	}
	if (desni != -1) {
		MPI_Cart_coords(cart, desni, 2, coords_d);
		printf("Desni sused proces %d sa koordinatama [%d, %d]\n", desni, coords_d[0], coords_d[1]);
	}

	MPI_Finalize();
	return 0;
}