#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

double funcion(float x);
float trapecio(float a, float b, float n,float h);

int main(int argc, char **argv){
    
    int  my_rank , p , n=1000 , local_n , source , dest=0 , tag=50;
    float a=0 , b=60 ,  h , local_a , local_b, integral , total;
    
    MPI_Status status;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    
    h= (b-a)/n; 
    local_n=n/p;
    
    local_a=a+my_rank*local_n*h ; 
    local_b=local_a+local_n*h;
    
    integral=trapecio(local_a,local_b,local_n,h);
    printf("Soy el proceso %d y mi suma es %f\n", my_rank, integral);    
    
    if(my_rank==0) {
        total=integral;
        for(source=1;source<p;source++) {
             MPI_Recv(&integral,1,MPI_FLOAT,source,tag,MPI_COMM_WORLD,&status);
            total+=integral;
        };
        printf("With n= %d trapezoides\n la estimacion",n);
        printf("de la integral entre %f y %f\n es= %f \n",a,b,total);
    } else {
        MPI_Send(&integral,1,MPI_FLOAT,dest,tag,MPI_COMM_WORLD);
    };
    MPI_Finalize();
}

double f(float x){
    return sqrt(x);
}

float trapecio(float a, float b, float n,float h){
    float integral;
    float x;
    int i;
    
    integral = (f(a)+f(b))/2;
    x = a;
    for(i=1;i<n-1;i++){
        x+=h;
        integral+=f(x);
    }
    integral*=h;
    return integral;
}