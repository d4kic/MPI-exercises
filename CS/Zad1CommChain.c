/* zadatak. Napisati program koji uzima podatke od nultog procesa i šalje ih svim
drugim procesima tako što proces
i treba da primi podatke i pošalje ih procesu
i+1, sve dok se ne stigne do poslednjeg procesa. Unos podataka se završava
nakon što se prenese negativna vrednost podatka. */

#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status status;
	int a = 0;
	do
	{
		if (rank == 0)
		{
			scanf("%d", &a);
			MPI_Send(&a, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		}
		else
		{
			MPI_Recv(&a, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
			if (rank != size - 1)
				MPI_Send(&a, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		}
		printf("Process %d, value = %d\n", rank, a);
		fflush(stdout);
	} while (a >= 0);
	MPI_Finalize();
	return 0;
}