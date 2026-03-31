#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if (size != 2)
	{
		if (rank == 0) printf("Program supports only 2 processes!\n");
		MPI_Finalize();
		return 0;
	}

	const int n = 1000;
	float sendBuff[n], recvBuff[n];
	MPI_Status status;
	for (int i = 0; i < n; ++i)
		sendBuff[i] = rank;

	if (rank == 0)
	{
		MPI_Send(sendBuff, n, MPI_FLOAT, 1, 1, MPI_COMM_WORLD);
		MPI_Recv(recvBuff, n, MPI_FLOAT, 1, 2, MPI_COMM_WORLD, &status);
	}
	else
	{
		MPI_Recv(recvBuff, n, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Send(sendBuff, n, MPI_FLOAT, 0, 2, MPI_COMM_WORLD);
	}

	printf("Process %d, received: %.2f", rank, recvBuff[0]);

	MPI_Finalize();
	return 0;
}