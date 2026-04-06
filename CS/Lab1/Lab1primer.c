/* Primer zadatka. Napisati MPI program u kome root proces (P0) treba da podeli ulazni niz
duzine p*(p+1) elemenata (p je broj procesa radnika) na delove tako sto se svakom procesu
radniku salje deo niza duzine 2*i, gde je i identifikator procesa (npr. ako imamo 3 procesa
radnika i niz je duzine 12, onda proces 1 dobija prva dva elementa niza, proces 2 sledeca 4
elementa, a proces 3 poslednjih 6 elemenata niza). Procesi radnici treba da pronadju sumu
vrednosti iz svog dela niza i odstampaju je na ekranu zajedno sa svojim identifikatorom.
Koristiti celobrojne nizove. Zadatak realizovati koriscenjem MPI Point-to-Point operacija. */

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	const int p = size - 1;
	if (rank == 0)
	{
		const int n = p * (p + 1);
		int* niz = (int*)malloc(n * sizeof(int));
		for (int i = 0; i < n; ++i)
			niz[i] = i + 1;
		int offset = 0;
		for (int i = 1; i <= p; ++i)
		{
			int count = 2 * i;
			MPI_Send(niz + offset, count, MPI_INT, i, 0, MPI_COMM_WORLD);
			offset += count;
		}
		free(niz);
	}
	else
	{
		int count = 2 * rank;
		int* niz = (int*)malloc(count * sizeof(int));
		MPI_Recv(niz, count, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		int sum = 0;
		for (int i = 0; i < count; ++i)
			sum += niz[i];
		printf("Process %d, sum = %d", rank, sum);
		free(niz);
	}
	MPI_Finalize();
	return 0;
}