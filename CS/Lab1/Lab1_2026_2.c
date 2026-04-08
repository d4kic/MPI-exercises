#include <stdio.h>
#include <mpi.h>
#include <string.h>

#define len 21

void reverseString(char* str)
{
	int n = strlen(str);
	for (int i = 0; i < n / 2; ++i)
	{
		char tmp = str[i];
		str[i] = str[n - i - 1];
		str[n - i - 1] = tmp;
	}
}

int main(int argc, char** argv)	
{
	int rank, numproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	char msg[len] = "laboratorijska vezba";
	char buff[len];
	MPI_Status status;
	MPI_Request req;

	double start = MPI_Wtime();
	if (rank == 0)
	{
		for (int i = 1; i < numproc; ++i)
			MPI_Send(msg, len, MPI_CHAR, i, 0, MPI_COMM_WORLD);
		for (int i = 1; i < numproc; ++i)
		{
			MPI_Recv(buff, len, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Process 0 primio %s\n", buff);
		}
	}
	else
	{
		MPI_Recv(buff, len, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		reverseString(buff);
		MPI_Send(buff, len, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}
	double end = MPI_Wtime();
	if (rank == 0)
		printf("Blocking Time: %f\n", end - start);

	MPI_Barrier(MPI_COMM_WORLD);
	start = MPI_Wtime();
	if (rank == 0)
	{
		for (int i = 1; i < numproc; ++i)
		{
			MPI_Isend(msg, len, MPI_CHAR, i, 0, MPI_COMM_WORLD, &req);
			MPI_Wait(&req, &status);
		}
		for (int i = 1; i < numproc; ++i)
		{
			MPI_Irecv(buff, len, MPI_CHAR, i, 0, MPI_COMM_WORLD, &req);
			MPI_Wait(&req, &status);
			printf("Process 0 primio %s\n", buff);
		}
	}
	else
	{
		MPI_Irecv(buff, len, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &req);
		MPI_Wait(&req, &status);
		reverseString(buff);
		MPI_Isend(buff, len, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &req);
		MPI_Wait(&req, &status);
	}
	end = MPI_Wtime();
	if (rank == 0)
		printf("Non-blocking Time: %f\n", end - start);

	MPI_Finalize();
	return 0;
}
