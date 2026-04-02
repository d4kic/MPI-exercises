/* zad. Napisati MPI program koji pronalazi proizvod matrice Anxn i vektora
bn. Matrica A i vector b se inicijalizuju u procesu 0. Izracunavanje se
obavlja tako što se svakom procesu distribuira po vrsta matrice A i ceo
vektor b. Svi procesi ucestvuju u izracunavanju. Rezultat se prokazuje u
procesu 0. */

#include <stdio.h>
#include <mpi.h>

#define n 6

int main(int argc, char** argv)
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int a[n][n], b[n];
	if (rank == 0)
	{
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				a[i][j] = i + j;
		for (int i = 0; i < n; ++i)
			b[i] = 1;
	}
	int local_a[n], local_c = 0, c[n];
	MPI_Scatter(a, n, MPI_INT, local_a, n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(b, n, MPI_INT, 0, MPI_COMM_WORLD);
	for (int i = 0; i < n; ++i)
		local_c += local_a[i] * b[i];
	MPI_Gather(&local_c, 1, MPI_INT, c, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		for (int i = 0; i < n; ++i)
			printf("%d ", c[i]);
	}
	MPI_Finalize();
	return 0;
}