#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

#define MAX 10

float funcion(float x);
float TrapecioCompuesto(int a, int b,  int n, float h);


int main(int argc, char **argv)
{
	/* code */
	int a = 0, b=60, n=1000 , local_n , source , dest=0 , tag=50, p, my_rank;
	float resultado, local_a , local_b , total, h;

   	MPI_Status status;

   	MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);

	h = (b-a)/n;
	local_n=n/p;
    
    local_a=a+my_rank*local_n*h ; 
    local_b=local_a+local_n*h;
    
    resultado = TrapecioCompuesto(a,b, local_n,h);
    printf("Soy el proceso %d y mi suma es %f\n", my_rank, resultado);    
    
    if(my_rank==0) {
        total=resultado;
        for(source=1;source<p;source++) {
             MPI_Recv(&resultado,1,MPI_FLOAT,source,tag,MPI_COMM_WORLD,&status);
            total+=resultado;
        };
        printf("With n= %d trapezoides\n la estimacion",n);
        printf("de la resultado entre %d y %d\n es= %f \n",a,b,total);
    } else {
        MPI_Send(&resultado,1,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
    };
   
	MPI_Finalize();
	return 0;
}

float funcion(float x){
	//return sqrt(x);
	return x*2;
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