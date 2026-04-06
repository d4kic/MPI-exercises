#include <iostream>
#include <mpi.h>
#include <cstdlib>

#define BUFF_COUNT 5000

int main(int argc, char** argv)
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	double buff[BUFF_COUNT];
	memset(buff, 0, sizeof(buff));
	for (int rep = 0; rep < 1000; ++rep) 
	{
		for (int i = 0; i < BUFF_COUNT; ++i) 
		{
			double val = (double)rand() / RAND_MAX;
			buff[i] += val;
		}
	}

	double reception[BUFF_COUNT];
	MPI_Reduce(buff, reception, BUFF_COUNT, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0)
		for (int i = 0; i < BUFF_COUNT; ++i)
			printf("%.2lf\n", reception[i]);

	MPI_Finalize();
	return 0;
}
