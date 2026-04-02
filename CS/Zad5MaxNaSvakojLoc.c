/* zad. Svaki od N procesa sadrži 30 realnih brojeva. Napisati MPI
program koji nalazi maksimalnu vrednost na svakoj od 30 lokacija, kao
i identifikator procesa koji sadrzi tu vrednost. */

#include <stdio.h>
#include <mpi.h>

#define n 30

struct buf
{
	double val;
	int rank;
};

int main(int argc, char** argv)
{
	int rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	struct buf in[n], out[n];
	for (int i = 0; i < n; ++i)
	{
		in[i].val = rank + i;
		in[i].rank = rank;
	}
	MPI_Reduce(&in, &out, n, MPI_DOUBLE_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
	if (rank == 0)
		for (int i = 0; i < n; ++i)
			printf("Val = %.2lf, rank = %d\n", out[i].val, out[i].rank);
	MPI_Finalize();
	return 0;
}