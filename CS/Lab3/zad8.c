#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>

#define k 5
#define s 5

// mora da se pokrene sa k*s procesa
int main(int argc, char** argv) {
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int dims[2] = { k, s }, periods[2] = { 1, 0 }, coords[2];
	
	MPI_Comm cart;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart);
	MPI_Cart_coords(cart, rank, 2, coords);
	int b = 10 * coords[0] + coords[1];
	int b_pre = b;

	int src, dest;
	MPI_Cart_shift(cart, 0, s - 1, &src, &dest);
	MPI_Sendrecv_replace(&b, 1, MPI_INT, dest, 0, src, 0, cart, MPI_STATUS_IGNORE);

	printf("Proces %d, b pre razmene = %d, posle razmene b = %d\n", rank, b_pre, b);

	MPI_Finalize();
	return 0;
}