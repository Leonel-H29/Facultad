#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int nproc, my_id;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    int *globaldata=NULL;
    int localdata;

    if (my_id == 0) {
        globaldata = malloc(nproc * sizeof(int) );
        for (int i=0; i<nproc; i++)
            globaldata[i] = 2*i+1;

        printf("Procesador %d tiene los datos: ", my_id);
        for (int i=0; i<nproc; i++)
            printf("%d ", globaldata[i]);
        printf("\n");
    }

    MPI_Scatter(globaldata, 1, MPI_INT, &localdata, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Procesador %d tiene el dato %d\n", my_id, localdata);
    localdata *= 2;
    
    MPI_Gather(&localdata, 1, MPI_INT, globaldata, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_id == 0) {
        printf("Procesador %d tiene los datos: ", my_id);
        for (int i=0; i<nproc; i++)
            printf("%d ", globaldata[i]);
        printf("\n");
    }

    if (my_id == 0)
        free(globaldata);

    MPI_Finalize();
    return 0;
}