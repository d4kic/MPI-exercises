/* zadatak. Napisati MPI program koji izracunava vrednost skalarnog proizvoda
dva vektora dimenzije N. Pretpostaviti da je N deljivo sa brojem procesa.
Vrednosti vektora a i b se ucitavaju u procesu P0. */

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define n 6

int main(int argc, char** argv)
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	double a[n], b[n];
	int perProc = n / numproc; // Broj clanova koje ce svaki proces da obradjuje
	double* local_a = (double*)malloc(perProc * sizeof(double));
	double* local_b = (double*)malloc(perProc * sizeof(double));
	if (rank == 0)
	{
		for (int i = 0; i < n; ++i)
			scanf("%lf", &a[i]);
		for (int i = 0; i < n; ++i)
			scanf("%lf", &b[i]);
	}
	MPI_Scatter(a, perProc, MPI_DOUBLE, local_a, perProc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(b, perProc, MPI_DOUBLE, local_b, perProc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	double local_res = 0, res;
	for (int i = 0; i < perProc; ++i)
		local_res += local_a[i] * local_b[i];
	MPI_Reduce(&local_res, &res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
		printf("Skalarni proizvod je %.2lf", res);
	free(local_a);
	free(local_b);
	MPI_Finalize();
	return 0;
}