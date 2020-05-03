#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char **argv){
        int my_rank;
        int np;
        int source;
        int dest;
        int tag = 0;
        char message[100];
	int m = -1;
	int n = -1;

        printf("prog:mpi-test:\n");

        MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
        MPI_Comm_size(MPI_COMM_WORLD, &np);

	#pragma omp parallel
	{
	//m = omp_get_max_threads();
	n = omp_get_thread_num();
	m = omp_get_num_threads();

        printf("threads:%d/%d:\n",n,m);
        printf("rank:%d:\n",my_rank);
        printf("nproc:%d:\n",np);
	}

        MPI_Finalize();

}

