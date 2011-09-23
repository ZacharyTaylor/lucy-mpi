/*
"Hello World" Type MPI Test Program
 */
#include <mpi.h>
#include <CImg.h>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "lucy.h"
#include "mpi_behaviour.h"
#include "files_io.h"

#define TAG_PSF 0
#define TAG_DATA 1

using namespace std;

void master_program_main(int numprocs) {

    vector<string> files = vector<string>();

    init_images(files);

    cimg_library::CImg<double> f_psf = get_psf();

    for(int i = 1; i < numprocs; i++){
        mpi_image_send(f_psf, i, TAG_PSF);
    }
}

void slave_program_main(MPI_Status stat) {
    cimg_library::CImg<double> f_psf = mpi_image_receive(TAG_PSF, stat);
    cimg_library::CImgDisplay main_disp(f_psf, "Base Image");

    while (!main_disp.is_closed()) {
    }
}

int main(int argc, char *argv[]) {
    char idstr[32];
    int numprocs;
    int myid;
    int i;
    MPI_Status stat;

    MPI_Init(&argc, &argv); // all MPI programs start with MPI_Init; all 'N'
    //processes exist thereafter
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs); // find out how big the SPMD
    //world is
    MPI_Comm_rank(MPI_COMM_WORLD, &myid); // and this processes' rank is

    if (myid == 0) {
        master_program_main(numprocs);
    } else {
        slave_program_main(stat);
    }

    MPI_Finalize(); // MPI Programs end with MPI Finalize; this is a weak
    //synchronization point
    return 0;
}