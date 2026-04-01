/* Napisati MPI program koji izračunava sumu N celih brojeva (N je
stepen 2) korišćenjem Point-to-point komunikacije.
U prvom koraku, procesi se grupišu u parove (P0, P1), (P2,
P3),…, (Pp-2, Pp-1). Zatim se izačunavaju parcijalne sume u
svim parovima korišćenjem P-to-P komunikacije i akumuliraju u
procesima (P0, P2,…, Pp-2). Npr. process Pi (i-parno) izračunava
parcijalne sume za par procesa (Pi , Pi+1).
U sledećem koraku razmatraju se parovi procesa (P0, P2), (P4,
P6),…, (Pp-4, Pp-2)pronalaze parcijalne sume i akumuliraju u
(P0, P4,…, Pp-4). Postupak se ponavlja dok ne ostanu 2 procesa
i rezultat se akumulira u P0. */

#include <stdio.h>
#include <mpi.h>

// NAPOMENA: Podrazumeva se da je N stepen 2!!!
int main(int argc, char** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status status;
	int val = rank; // Ovde ce da se racuna zbir
	int step = 1; // Bolji nacin da se nalazi sused, posto je u svakom koraku on udaljen za stepen dvojke
	while (step != size)
	{
		if (rank % (2 * step) == 0) // Izdvajanje potrebnih procesa u kojima se vrsi sabiranje
		{
			if (rank + step < size) // Nalazi onog sto prima vrednost od suseda
			{
				int recv;
				MPI_Recv(&recv, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD, &status);
				val += recv;
			}
		}
		else // Ako nije potreban za racunanje u tr. koraku, salje i izlazi
		{
			MPI_Send(&val, 1, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
			break;
		}
		step *= 2;
	}
	if (rank == 0)
		printf("Suma je %d", val);
	MPI_Finalize();
	return 0;
}