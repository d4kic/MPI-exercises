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
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int p = numproc - 1; // p0 nije proces radnik
	if (rank == 0)
	{
		const int n = p * (p + 1);
		int* niz = (int*)malloc(n * sizeof(int));
		for (int i = 0; i < n; ++i)
			niz[i] = i + 1;
		int offset = 0;
		for (int i = 1; i < numproc; ++i)
		{
			MPI_Send(niz + offset, i * 2, MPI_INT, i, 0, MPI_COMM_WORLD);
			offset += i * 2;
		}
	}
	else
	{
		int recvCount = rank * 2, sum = 0;
		int* niz = (int*)malloc(recvCount * sizeof(int));
		MPI_Recv(niz, recvCount, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < recvCount; ++i)
			sum += niz[i];
		printf("Process %d je izracunao %d", rank, sum);
	}
	MPI_Finalize();
	return 0;
}
