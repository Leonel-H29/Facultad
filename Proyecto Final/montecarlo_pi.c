/*
Metodo de Montecarlo para estimar el valor de PI
Author: Herrera, Leonel Esteban
*/


#include <math.h>
#include <limits.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
/* no. of random nos. to generate at one time */
#define CHUNKSIZE 1000
/* message tags */
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

	if (numprocs==1)
		printf("Error. At least 2 nodes are needed");
	/* process 0 reads epsilon from args and broadcasts it toeveryone */

	if (myid == 0) {
		if (argc<2) {
			epsilon = 1e-2;
		} 
		else {
			sscanf ( argv[1], "%lf", &epsilon );
		}
	}
	MPI_Bcast ( &epsilon, 1, MPI_DOUBLE, 0,MPI_COMM_WORLD );
	/* define the workers communicator by using groups and
	excluding the server from the group of the whole world */
	MPI_Comm_group ( world, &world_group );
	ranks[0] = server;
	MPI_Group_excl ( world_group, 1, ranks, &worker_group );
	MPI_Comm_create ( world, worker_group, &workers);
	MPI_Group_free ( &worker_group);

	/* the random number server code - receives a non-zero
	request, generates random numbers into the array rands,
	and passes them back to the process who sent the
	request. */

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
		/* the code for the worker processes - each one sends a
		request for random numbers from the server, receives
		and processes them, until done */
	} 
	else {
		request = 1;
		done = in = out = 0;
		max = INT_MAX;
		/* send first request for random numbers */
		MPI_Send( &request, 1, MPI_INT, server, REQUEST, world );
		/* all workers get a rank within the worker group */
		MPI_Comm_rank ( workers, &workerid );
		iter = 0;
		while (!done) {
			iter++;
			request = 1;
			/* receive the chunk of random numbers */
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
			/* the value of in is sent to the variable totalin in
			all processes in the workers group */
			MPI_Allreduce(&in, &totalin, 1,MPI_INT, MPI_SUM, workers);
			MPI_Allreduce(&out, &totalout, 1,MPI_INT, MPI_SUM, workers);
			Pi = (4.0*totalin)/(totalin + totalout);
			error = fabs ( Pi - M_PI);
			done = ((error < epsilon) || ((totalin+totalout)>1000000));
			request = (done) ? 0 : 1;
			/* if done, process 0 sends a request of 0 to stop the
			rand server, otherwise, everyone requests more
			random numbers. */
			if (myid == 0) {
				printf("pi = %23.20lf\n", Pi );
				MPI_Send( &request, 1, MPI_INT, server, REQUEST, world);
			} 
			else {
				if (request)
					MPI_Send(&request, 1, MPI_INT, server, REQUEST, world);
				}
			}
		}
		if (myid == 0)
			printf("total %d, in %d, out %d\n",
			totalin+totalout, totalin, totalout );
		if (myid<server) MPI_Comm_free(&workers);
		MPI_Finalize();
}