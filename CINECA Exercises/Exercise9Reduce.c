#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define n 5

int main(int argc, char** argv)
{
	int rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int arr[n], arr_sum[n], arr_prod[n];
	for (int i = 0; i < n; ++i)
		arr[i] = rank + 1;
	MPI_Reduce(arr, arr_sum, n, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(arr, arr_prod, n, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		for (int i = 0; i < n; ++i)
		{
			printf("arr_sum[%d] = %d, arr_prod[%d] = %d\n", i, arr_sum[i], i, arr_prod[i]);
		}
	}
	srand(time(0));
	int r_max, r = rand() % 10 + rank;
	printf("Process id %d has r = %d\n", rank, r);
	MPI_Reduce(&r, &r_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) printf("Max random val is %d", r_max);
	MPI_Finalize();
	return 0;
}
