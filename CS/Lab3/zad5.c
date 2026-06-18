#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>

#define k 5

// mora da se pokrene sa k*k procesa
int main(int argc, char** argv) {
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int dims[2] = { k, k }, periods[2] = { 0, 0 }, coords[2], color;

	MPI_Comm cart, diag;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart);
	MPI_Cart_coords(cart, rank, 2, coords);

	color = (coords[0] == coords[1]) ? 1 : 0;
	MPI_Comm_split(cart, color, rank, &diag);

	if (color == 1) {
		int nrank, nnumproc;
		MPI_Comm_rank(diag, &nrank);
		MPI_Comm_size(diag, &nnumproc);

		char buff[100];
		if (nrank == 0) {
			strcpy(buff, "Poruka");
		}

		MPI_Bcast(buff, sizeof(buff), MPI_CHAR, 0, diag);
		printf("Proces %d, primio poruku: %s\n", rank, buff);
	}

	MPI_Finalize();
	return 0;
}