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
	int a = 0, b=60, n=1000 , local_n , source , dest=0 , tag=50, p;
	float resultado, local_a , local_b, integral , total, h;

	//int my_id, nproc, tag = 1, source;
   	MPI_Status status;

   	MPI_Init(&argc, &argv);
   	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
   	MPI_Comm_size(MPI_COMM_WORLD, &nproc);

	h = (b-a)/n;
	local_n=n/p;
    
    local_a=a+my_rank*local_n*h ; 
    local_b=local_a+local_n*h;
    
    resultado = TrapecioCompuesto(a,b, local_n);
    printf("Soy el proceso %d y mi suma es %f\n", my_rank, resultado);    
    
    if(my_rank==0) {
        total=integral;
        for(source=1;source<p;source++) {
             MPI_Recv(&integral,1,MPI_FLOAT,source,tag,MPI_COMM_WORLD,&status);
            total+=integral;
        };
        printf("With n= %d trapezoides\n la estimacion",n);
        printf("de la integral entre %f y %f\n es= %f \n",a,b,total);
    } else {
        MPI_Send(&resultado,1,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
    };
   
	MPI_Finalize();
	return 0;
}

float funcion(float x){
	return sqrt(x);
}

float TrapecioCompuesto(int a, int b, int n, float h){
	
	float sumatoria = 0;

	for (int i = 1; i < n; ++i)
	{
		sumatoria = sumatoria + funcion(a + h*i);
		/* code */
	}
	return ((h/2) * (funcion(a)+2*sumatoria + funcion(b)));
}