/* zad. Napisati MPI program koji nalazi minimalnu i maximalnu vrednost zadate
promenljive za N procesa kao i identifikatore procesa koji sadrže te vrednosti. */

#include <stdio.h>
#include <mpi.h>

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
	struct buf in, out;
	in.val = rank + 1;
	in.rank = rank;
	int rootMax = 0, rootMin = 1;
	MPI_Reduce(&in, &out, 1, MPI_DOUBLE_INT, MPI_MAXLOC, rootMax, MPI_COMM_WORLD);
	MPI_Reduce(&in, &out, 1, MPI_DOUBLE_INT, MPI_MINLOC, rootMin, MPI_COMM_WORLD);
	if (rank == rootMax)
		printf("Process %d has found the max value %lf", rank, out.val);
	if (rank == rootMin)
		printf("Process %d has found the min value %lf", rank, out.val);
	MPI_Finalize();
	return 0;
}