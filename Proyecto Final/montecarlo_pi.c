#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
main(int argc, char** argv)
{
        // Definiciones 
        double x,y,z,pi;
        int i,count = 0,mycount,iterations = 100000;
        int rank,size,proc;
        MPI_Status status;
        // Iniciar MPI
        MPI_Init(&argc,&argv);
        // Numero de procesos
        MPI_Comm_size(MPI_COMM_WORLD,&size);
        // Rango del proceso
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);
        // El proceso 0 se encarga de recibir los conteos de los otros procesos y estimar pi
        if (rank == 0) {
                // El conteo es 0 inicialmente
                count = 0;
                // Para cada proceso recibira el conteo que han realizado y lo sumará
                for (proc=1; proc < size; proc++) {
                 MPI_Recv(&mycount,1,MPI_REAL,proc,1,MPI_COMM_WORLD,&status);
                 count +=mycount;
                }
                // Estimacion de pi, el tamaño de puntos generados sera igual al numero de procesos menos el proceso 0 por el numero de iteraciones
                pi = (double) 4*count/((size - 1)*iterations);
                // Imprimir en pantalla
                printf("Cantidad de Iteraciones = %d, Pi estimado: %g \n",(size - 1)*iterations,pi);
        }
        // Si no es el proceso 0 entonces generar las coordenadas aleatorias y verificar si estan en el circulo para hacer el conteo
        else {
                // Semilla diferente para cada proceso que dependera en este caso del rango
                srand(rank);
                // El conteo de cada proceso inicia en 0
                mycount = 0;
                for(i = 0;i < iterations;i++){
                  // Coordenada x
                  x = (double) rand()/RAND_MAX;
                  // Coordenada y
                  y = (double) rand()/RAND_MAX;
                  // Distancia al centro
		  		  z = x*x+y*y;
                  // Verificar si la coordenada esta dentro del circulo
                  if(z <= 1) mycount++;
                }
                // Imprimir en pantalla
                printf("Proceso %d enviando un conteo total de = %d al proceso 0\n",rank,mycount);
                // Enviar conteo al proceso 0
                MPI_Send(&mycount,1,MPI_REAL,0,1,MPI_COMM_WORLD);
        }
        // Finalizar MPI
        MPI_Finalize();
        // Finalizar
        return 0;
}