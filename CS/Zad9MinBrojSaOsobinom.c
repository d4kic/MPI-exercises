/* zad. Napisati MPI program koji pronalazi i prikazuje minimalni neparan broj sa zadatom osobinom i
identifikator procesa koji ga sadrzi. Neparni brojevi se nalaze u intervalu [a,b](a i b su zadate
konstante). Osobina koju broj treba da poseduje je da je deljiv zadatom vrednoscu x. Prilikom
ispitivanja (da li broj poseduje zadatu osobinu ili ne) svaki proces generise i ispituje odgovarajuce
neparne brojeve na nacin prikazan na slici (za primer broj_procesa=4 i a=3, b=31, x=5). Konacne
rezultate treba da prikaze proces koji sadrži najmanji broj takvih brojeva. Zadatak resiti koriscenjem
grupnih operacija. */

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define a 3
#define b 31
#define x 5

struct buff {
	int val;
	int rank;
};

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);
	int rank, numproc;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numproc);
	int count = 0, min = INT_MAX;
	struct buff tmp1, tmp2, minval, mincount;
	for (int i = a + rank * 2; i < b; i += numproc) {
		if (i % x == 0) {
			count++;
			if (i < min) {
				min = i;
			}
		}
	}
	tmp1.rank = tmp2.rank = rank;
	tmp1.val = count;
	MPI_Reduce(&tmp1, &mincount, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD); // trazi se element sa najmanji count
	MPI_Bcast(&mincount, 1, MPI_2INT, 0, MPI_COMM_WORLD); // svima se salje info o element sa najmanjim count
	tmp2.val = min;
	MPI_Reduce(&tmp2, &minval, 1, MPI_2INT, MPI_MINLOC, mincount.rank, MPI_COMM_WORLD); // trazi se minimalni broj koji ispunjava uslov
	if (rank == mincount.rank) {
		printf("ID procesa koji prikazuje rezultate %d, minimalna vrednost je %d.", mincount.rank, minval.val);
	}
	MPI_Finalize();
	return 0;
}