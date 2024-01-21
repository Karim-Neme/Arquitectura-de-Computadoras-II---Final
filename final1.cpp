#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<omp.h>

using namespace std;

int main(int argc, char* argv[]) {
 
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int numenv=20;
    int numrec;
    
    //Instrucciones para el proceso 1
    if(rank==1){
        
        //Recibe el numero el proceso 0 envie
        MPI_Recv(&numrec,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        
        //Mediante el numero seleccionado de hilos se reparten el trabajo de ir disminuyendo el numero recibido hasta 0
        #pragma omp parallel num_threads(5)
        {
            
            #pragma omp for
            for (int i=0 ; i<=numenv ; i++)
            {
                #pragma omp critical
                {
                    int hilonum = omp_get_thread_num();
                    printf("Proceso %d, hilo %d muestra el numero %d \n", rank, hilonum, numrec);
                    numrec--;
                }
            }
        }
        
    //Instrucciones para el proceso 0    
    }else if(rank==0){
        
        //Envia un numero al proceso 1
        MPI_Send(&numenv,1,MPI_INT,1,0,MPI_COMM_WORLD);
        printf("El proceso %d envia el numero %d \n\n", rank, numenv);
    }
    
    MPI_Finalize();
    
    return 0;
    
}