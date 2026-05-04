/* zad. Napisati MPI program koji pronalazi proizvod matrice Amxn i vektora
bn. Matrica A i vector b se inicijalizuju u procesu 0. Izracunavanje se
obavlja tako što se svakom procesu distribuira po kolona matrice A i po 1
element vektora b. Za distribuciju kolona po procesima koristiti P-t-P
operacije, za sve ostalo grupne operacije. Svi procesi ucestvuju u
izracunavanju. Rezultat se prikazuje u procesu koji, nakon distribuiranja
kolona matrice A, sadrzi minimum svih elemenata matrice A. */

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define n 3
#define m 4

struct buff
{
	int val;
	int rank;
};

int main(int argc, char** argv)
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int a[n][m], b[m];
	int x[n], y[n], z;
	struct buff min, local_min;
	if (rank == 0)
	{
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				a[i][j] = i + j;
			}
		}
		for (int i = 0; i < m; ++i)
		{
			b[i] = 1;
		}
	}
	if (rank == 0)
	{
		for (int i = 0; i < n; ++i)
			x[i] = a[i][0];
		for (int j = 1; j < numproc; ++j)
		{
			for (int i = 0; i < n; ++i)
			{
				y[i] = a[i][j];
			}
			MPI_Send(y, n, MPI_INT, j, 0, MPI_COMM_WORLD);
		}
	}
	else
		MPI_Recv(x, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Scatter(&b[0], 1, MPI_INT, &z, 1, MPI_INT, 0, MPI_COMM_WORLD);
	int local_res[n], res[n];
	for (int i = 0; i < n; ++i)
		local_res[i] = x[i] * z;
	local_min.val = INT_MAX;
	for (int i = 0; i < n; ++i)
	{
		if (x[i] < local_min.val)
		{
			local_min.val = x[i];
			local_min.rank = rank;
		}
	}
	MPI_Reduce(&local_min, &min, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&min, 1, MPI_2INT, 0, MPI_COMM_WORLD);
	MPI_Reduce(local_res, res, n, MPI_INT, MPI_SUM, min.rank, MPI_COMM_WORLD);
	if (rank == min.rank)
	{
		for (int i = 0; i < n; ++i)
		{
			printf("res[%d] = %d\n", i, res[i]);
		}
		printf("min = %d\n", min.val);
	}
	MPI_Finalize();
	return 0;
}