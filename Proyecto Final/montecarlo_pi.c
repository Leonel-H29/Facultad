/*
Metodo de Montecarlo para estimar el valor de PI
Author: Herrera, Leonel Esteban
*/


#include <math.h>
#include <limits.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
/*Numero de numeros ramdon para generar a la vez*/
#define CHUNKSIZE 1000
/* Etiquetas de mensajes */
#define REQUEST 1
#define REPLY 2
int main(int argc, char **argv) {
	int iter;
	int in, out, i, iters, max, ix, iy, ranks[1], done, temp;
	double x, y, Pi, error, epsilon;
	int numprocs, myid, server, totalin, totalout, workerid;
	int rands[CHUNKSIZE], request;

	MPI_Comm world, workers;
	MPI_Group world_group, worker_group;
	MPI_Status stat;
	MPI_Init(&argc,&argv);
	world = MPI_COMM_WORLD;
	MPI_Comm_size(world, &numprocs);
	MPI_Comm_rank(world, &myid);
	server = numprocs-1;

	//printf("Numero de procesos: %d\n", numprocs );

	if (numprocs==1)
		printf("Error. Al menos 2 nodos se necesitan");
	/*El proceso 0 lee el epsilon desde los argumentos y los transmite a todos*/

	if (myid == 0) {
		if (argc<2) {
			epsilon = 1e-2;
		} 
		else {
			sscanf ( argv[1], "%lf", &epsilon );
		}
	}
	MPI_Bcast ( &epsilon, 1, MPI_DOUBLE, 0,MPI_COMM_WORLD );
	/* definir el comunicador de los trabajadores mediante el uso de grupos y
	excluyendo al servidor del grupo de todo el mundo */

	MPI_Comm_group ( world, &world_group );
	ranks[0] = server;
	MPI_Group_excl ( world_group, 1, ranks, &worker_group );
	MPI_Comm_create ( world, worker_group, &workers);
	MPI_Group_free ( &worker_group);

	/*
	El codigo del servidor del numero aleatorio - recibe una solicitud no nula,
	genera numeros aleatorios dentro del los intervalos del arreglo y los devuelve
	al proceso quien envia la solicitud
	*/

	if ( myid == server ) {
		do {
			MPI_Recv(&request, 1, MPI_INT, MPI_ANY_SOURCE,
			REQUEST, world, &stat);
			if ( request ) {
				for (i=0; i<CHUNKSIZE; i++) rands[i] = random();
				MPI_Send(rands, CHUNKSIZE, MPI_INT,
				stat.MPI_SOURCE, REPLY, world);
			}
		} while ( request > 0 );
		/*
		El codigo de los procesos de los trabajadores - cada uno envia
		una solicitud de numeros aleatorios desde el servidor, reciben y procesan
		estas, hasta terminar
		*/
	} 
	else {
		request = 1;
		done = in = out = 0;
		max = INT_MAX;
		/*Envia la primer solicitud de numeros aleatorios*/
		MPI_Send( &request, 1, MPI_INT, server, REQUEST, world );
		/* Todos los trabajadores obtienen un rango dentro del grupo de trabajadores*/
		MPI_Comm_rank ( workers, &workerid );
		iter = 0;
		while (!done) {
			iter++;
			request = 1;
			/*Recibe las partes de los numeros aleatorios*/
			MPI_Recv(rands, CHUNKSIZE, MPI_INT, server,
			REPLY, world, &stat );
			for (i=0; i<CHUNKSIZE; ) {
				x = (((double) rands[i++])/max) * 2 - 1;
				y = (((double) rands[i++])/max) * 2 - 1;
				if (x*x + y*y < 1.0)
					in++;
				else
					out++;
			}
			/*
			El valor de in se envia a la variable totalin en todos los
			procesos del grupo de trabajadores
			*/
			MPI_Allreduce(&in, &totalin, 1,MPI_INT, MPI_SUM, workers);
			MPI_Allreduce(&out, &totalout, 1,MPI_INT, MPI_SUM, workers);
			Pi = (4.0*totalin)/(totalin + totalout);
			error = fabs ( Pi - M_PI);
			done = ((error < epsilon) || ((totalin+totalout)>1000000));
			request = (done) ? 0 : 1;

			/*
			Si se hace, el proceso 0 envia una solicitud de 0 a parar el
			 servidor rand, caso contrario, todos solicitan mas numeros aleatorios
			*/
			if (myid == 0) {
				/*Muestro el valor aproximado de pi*/
				printf("Valor aproximado de pi = %23.20lf\n", Pi );
				MPI_Send( &request, 1, MPI_INT, server, REQUEST, world);
			} 
			else {
				if (request)
					MPI_Send(&request, 1, MPI_INT, server, REQUEST, world);
				}
			}
		}
		if (myid == 0)
			/*
			Muestro el total de puntos obtenidos, mas los que estan dentro
			como fuera del circulo
			*/
			printf("Total de puntos:  %d,\n Dentro: %d,\n Fuera: %d\n",
			totalin+totalout, totalin, totalout );

			
		if (myid<server) MPI_Comm_free(&workers);

		printf("Numero de procesos: %d\n", numprocs );
		MPI_Finalize();
}