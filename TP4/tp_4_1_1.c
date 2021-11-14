#include <stdio.h>
#include <string.h> 
#include <mpi.h>
#define N 20

int main(int argc, char **argv)
{
  char mensaje[N];
  int  i, my_id, nproc;
  MPI_Status status;
  int root = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

  if (my_id == root)
  {
    strcpy(mensaje, "Hola mundo");
  }
  MPI_Bcast(mensaje, N, MPI_CHAR, root, MPI_COMM_WORLD);
  printf("Mensaje del proceso %d : %s\n", my_id, mensaje);

  MPI_Finalize();
}
