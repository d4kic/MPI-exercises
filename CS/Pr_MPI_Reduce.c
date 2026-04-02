#include <stdio.h>
#include <mpi.h>

struct ab
{
	int a;
	int b;
};

int main(int argc, char** argv)
{
	int rank;
	int root = 7;
	struct ab source, res;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	source.a = rank + 1;
	source.b = rank - 1;
	MPI_Reduce(&source, &res, 2, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
	if (rank == root)
		printf("Root process result is a=%d and b=%d", res.a, res.b);
	MPI_Finalize();
	return 0;
}