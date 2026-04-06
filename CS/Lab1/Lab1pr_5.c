#include <stdio.h>
#include <mpi.h>

#define n 5

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int buffer[n], sum = 0, flag;
	for (int i = 0; i < n; ++i)
	{
		buffer[i] = rank + i;
		sum += buffer[i];
	}
	if (rank != 0)
	{
		flag = sum % rank ? 1 : 0;
		MPI_Send(&flag, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else
	{
		for (int i = 1; i < size; ++i)
		{
			MPI_Recv(&flag, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (flag) printf("Proces %d ima adekvatnu sumu\n", i);
		}
	}
	MPI_Finalize();
	return 0;
}
