/**
 * MPI implementation of the Richardson-Lucy Algorithem. The master loads in the
 * images in ./images and passes them to the slaves for processsing. Upon being
 * returned the images are saved to ./Output. The program accepts .bmp and .jpg
 * images.
 * The images rather then there location are passed so that the slaves do not
 * require access to the location where the images are being read from and
 * stored to*/

//required includes
#include "master_slave.h"

//the main program
int main(int argc, char *argv[]) {
    int numprocs;
    int myid;

    MPI_Status stat;

    MPI_Init(&argc, &argv); // all MPI programs start with MPI_Init; all 'N'
    //processes exist thereafter
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs); // find out how big the SPMD
    //world is
    MPI_Comm_rank(MPI_COMM_WORLD, &myid); // and this processes' rank is

    //if a master run this
    if (myid == 0) {
        //start the total time timer
        double start = (clock() / (double) CLOCKS_PER_SEC);

        master_program_main(stat, numprocs);

        //stop the total time timer
        double finish = (clock() / (double) CLOCKS_PER_SEC) - start;
        printf("Job done in %g seconds\n", finish);

    }
    //else a slave so run this
    else {
        slave_program_main(stat);
    }

    printf("process %i finished\n", myid);
    MPI_Finalize(); 

    return 0;
}