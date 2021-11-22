#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

#define MAX 10

float funcion(float x);
float TrapecioCompuesto(int a, int b);


int main(int argc, char const *argv[])
{
	/* code */
	int a = 0, b=60;
	float resultado;

	int my_id, nproc, tag = 1, source;
   	MPI_Status status;

   	MPI_Init(&argc, &argv);
   	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
   	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	
	resultado = TrapecioCompuesto(a,b, nproc);

	printf("El resultado es:  %f\n", resultado);

	MPI_Send(&my_id, 1, MPI_INT, (my_id+1)%nproc, tag, MPI_COMM_WORLD);
   	MPI_Recv(&source,1,MPI_INT,MPI_ANY_SOURCE,tag, MPI_COMM_WORLD, &status);

   printf("Soy el proceso %d y recibi un mensaje de %d\n",my_id,source);


	MPI_Finalize();
	return 0;
}

float funcion(float x){
	return sqrt(x);
}

float TrapecioCompuesto(int a, int b, int n){
	int h = (int)((b-a)/h);
	float sumatoria = 0;

	for (int i = 1; i < n; ++i)
	{
		sumatoria = sumatoria + funcion(a + h*i);
		/* code */
	}
	return ((h/2) * (funcion(a)+2*sumatoria + funcion(b)));
}