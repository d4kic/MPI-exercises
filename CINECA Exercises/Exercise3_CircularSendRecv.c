#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	const int n = 10000;
	float sendBuff[n], recvBuff[n];
	MPI_Status status;
	MPI_Request req;
	for (int i = 0; i < n; ++i)
		sendBuff[i] = rank;

	int right = (rank + 1) % size;
	int left = (rank - 1 + size) % size;

	MPI_Isend(sendBuff, n, MPI_FLOAT, right, 0, MPI_COMM_WORLD, &req);
	MPI_Recv(recvBuff, n, MPI_FLOAT, left, 0, MPI_COMM_WORLD, &status);
	MPI_Wait(&req, &status);

	printf("Process %d receieved: %.2f", rank, recvBuff[0]);

	MPI_Finalize();
	return 0;
}