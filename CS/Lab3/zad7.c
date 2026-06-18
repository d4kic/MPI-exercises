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
	int gornji, donji;

	MPI_Comm cart;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart);
	MPI_Cart_coords(cart, rank, 2, coords);
	MPI_Cart_shift(cart, 0, 1, &gornji, &donji);

	int grank, drank;
	grank = (gornji != -1) ? gornji : 0;
	drank = (donji != -1) ? donji : 0;

	int prod = grank * drank, sum;
	MPI_Reduce(&prod, &sum, 1, MPI_INT, MPI_SUM, 0, cart);
	if (rank == 0) {
		printf("Suma proizvoda je %d\n", sum);
	}

	MPI_Finalize();
	return 0;
}