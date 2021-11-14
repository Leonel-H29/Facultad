#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

int prod_vectorial(int v1[], int v2[], int a, int b);

int main(int argc, char **argv)
{

	int arre1[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30}; 
	int arre2[] = {2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60}; 

	int tama = sizeof(arre1)/4;
	
	int dest = 0; //Proceso que realizara las sumas
	int tag;
	
	MPI_Status status;

	/*  Declaracion de variables  */
    int ID_Proceso, TotalProcesos, i, cant_pasos;
    int resultado_t, resultado_i;
    int nva_a, nva_b;//, 

	/*  MPI  */
	MPI_Init(&argc, &argv);              
    MPI_Comm_rank(MPI_COMM_WORLD, &ID_Proceso);  
    MPI_Comm_size(MPI_COMM_WORLD, &TotalProcesos);     
	

	cant_pasos = tama / TotalProcesos; 	
	
	nva_a = ID_Proceso * cant_pasos;
	nva_b = (ID_Proceso + 1) * cant_pasos;
	
	resultado_i = prod_vectorial(arre1, arre2, nva_a, nva_b);
	
	printf("Soy el proceso %d y mi suma es %d\n", ID_Proceso, resultado_i);	
	
	if(ID_Proceso == 0){
		resultado_t = resultado_i;
		for (i = 1; i < TotalProcesos; i++){
			MPI_Recv(&resultado_i, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
			resultado_t = resultado_t + resultado_i;
		}

		printf("\n El resultado es %d \n", resultado_t);
	}

	else
		MPI_Send(&resultado_i, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	
	MPI_Finalize();
	return 0;
}


int prod_vectorial(int v1[], int v2[], int a, int b){
	int i;
	int limite = b - a;
	int suma = 0;

	for(i = 0; i< limite; i++)
		suma = suma + v1[a + i] * v2[a + i];
	
	return suma;
}
