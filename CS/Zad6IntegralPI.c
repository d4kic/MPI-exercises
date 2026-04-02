/* zad.Napisati MPI program koji izracunava vrednost broja PI kao vrednost
integrala funkcije f(x)=4/(1+x2)^2 na intervalu [0,1] . */

#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int n;
	double mypi, pi, h, x, sum = 0;
	if (rank == 0)
		scanf("%d", &n);
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	h = 1.0 / n;
	for (int i = rank; i < n; i += numproc)
	{
		x = h * (i + 0.5);
		sum += 4.0 / (1.0 + x * x);
	}
	mypi = h * sum;
	MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
		printf("Pi is approx %lf", pi);
	MPI_Finalize();
	return 0;
}