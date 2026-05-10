/* zad. Svaki od N procesa sadrži 30 realnih brojeva. Napisati MPI
program koji nalazi maksimalnu vrednost na svakoj od 30 lokacija, kao
i identifikator procesa koji sadrzi tu vrednost. */

#include <stdio.h>
#include <mpi.h>

struct buf {
	int val;
	int rank;
};

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	int rank, numproc;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	struct buf in[30], out[30];
	for (int i = 0; i < 30; ++i) {
		in[i].rank = rank;
		in[i].val = rank + i;
	}
	MPI_Reduce(&in, &out, 30, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		for (int i = 0; i < 30; ++i) {
			printf("Element %d, rank %d ima max val = %d\n", i + 1, out[i].rank, out[i].val);
		}
	}
	MPI_Finalize();
	return 0;
}