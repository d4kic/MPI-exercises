#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define n 5
#define m 5

// mora da se pokrene sa n * m procesa
int main(int argc, char** argv) {
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int dims[2] = { n , m } , periods[2] = { 0,0 }, color, coords[2];
	MPI_Comm cart, grupa;
	
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart);
	MPI_Cart_coords(cart, rank, 2, coords);

	if (coords[0] < coords[1]) {
		color = 1; // gornja
	}
	else if (coords[0] > coords[1]) {
		color = 0; // donja
	}
	else {
		color = (rank % 2 == 0) ? 1 : 0;
	}

	MPI_Comm_split(cart, color, rank, &grupa);

	int local_res, res, nrank;
	MPI_Comm_rank(grupa, &nrank);

	local_res = rank;
	MPI_Reduce(&local_res, &res, 1, MPI_INT, MPI_SUM, 0, grupa);

	if (nrank == 0) {
		if (rank == 0) {
			int drugi_res;
			MPI_Recv(&drugi_res, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			int sum_gornja, sum_donja;
			if (color == 1) {
				sum_gornja = res;
				sum_donja = drugi_res;
			}
			else {
				sum_gornja = drugi_res;
				sum_donja = res;
			}

			printf("Suma gornja: %d\nSuma donja: %d\n", sum_gornja, sum_donja);
		}
		else {
			MPI_Send(&res, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
	return 0;
}