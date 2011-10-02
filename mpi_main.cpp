/*
"Hello World" Type MPI Test Program
 */
#include <mpi.h>
#include <CImg.h>
#include <stdio.h>
#include <string.h>
#include <vector>

#include "lucy.h"
#include "files_io.h"

#define TAG_PSF 0
#define TAG_DATA 1
#define TAG_END 2

#define IMAGE_WIDTH 1024
#define IMAGE_HEIGHT 1024

using namespace std;

void master_program_main(MPI_Status stat, int numprocs) {

    //get list of all images to process
    vector<string> files = vector<string > ();
    init_images(&files);

    //get forier transform of psf
    cimg_library::CImg<double> psf = get_psf();
    printf("loaded the psf\n");
    //send transformed psf to all the slaves
    for (int i = 1; i < numprocs; i++) {
        MPI_Send(psf._data, psf._height*psf._width, MPI_DOUBLE, i, TAG_PSF, MPI_COMM_WORLD);
    }
    printf("sent the psf\n");
    //initilize which images have been/ are being processed
    int processing[numprocs];
    unsigned int upto = 0;
    unsigned int doneto = 0;

    //start by sending an image to each slave
    for (int i = 1; i < numprocs; i++) {

        processing[i] = upto++;

        cimg_library::CImg<double> image = get_image(files, processing[i]);
        printf("sent %s to slave %i\n", files[processing[i]].c_str(), i);
        MPI_Send(image._data, image._height*image._width, MPI_DOUBLE, i, TAG_DATA, MPI_COMM_WORLD);
    }

    printf("master in while loop\n");

    bool all_done = false;
    while (!all_done) {
        for (int i = 1; i < numprocs; i++) {
            int flag = false;
            MPI_Iprobe(i, TAG_DATA, MPI_COMM_WORLD, &flag, &stat);
            if (flag == true) {
                //get output image
                cimg_library::CImg<double> output(IMAGE_WIDTH, IMAGE_HEIGHT, 1, 1, 0);
                MPI_Recv(output._data, IMAGE_WIDTH*IMAGE_HEIGHT, MPI_DOUBLE, i, TAG_DATA, MPI_COMM_WORLD, &stat);
                printf("master got %s back from slave %i and saved as output-%i\n", files[processing[i]].c_str(), i, processing[i]);

                //save it
                save_image(output, processing[i]);

                doneto++;

                printf("up to %i, done to %i  of %i\n", upto, doneto, files.size());
                if (upto != files.size()) {

                    processing[i] = upto++;

                    cimg_library::CImg<double> image = get_image(files, processing[i]);
                    printf("sent %s to slave %i\n", files[processing[i]].c_str(), i);

                    MPI_Send(image._data, image._height*image._width, MPI_DOUBLE, i, TAG_DATA, MPI_COMM_WORLD);
                } else if (doneto == files.size()) {
                    all_done = true;
                    for (int i = 1; i < numprocs; i++) {
                        int end = 0;
                        MPI_Send(&end, 1, MPI_INT, i, TAG_END, MPI_COMM_WORLD);
                    }
                }
            }
        }
    }


}

void slave_program_main(MPI_Status stat) {

    CImg<double> psf(3, 3, 1, 1, 0);
    MPI_Recv(psf._data, 9, MPI_DOUBLE, 0, TAG_PSF, MPI_COMM_WORLD, &stat);

    while (true) {
        printf("slave starting\n");

        int flag_image = false;
        int flag_end = false;

        while ((!flag_image) && (!flag_end)) {
            MPI_Iprobe(0, TAG_DATA, MPI_COMM_WORLD, &flag_image, &stat);
            MPI_Iprobe(0, TAG_END, MPI_COMM_WORLD, &flag_end, &stat);
        }

        //grab message and exit
        if (flag_end) {
            printf("killing slave\n");
            int end;
            MPI_Recv(&end, 1, MPI_INT, 0, TAG_END, MPI_COMM_WORLD, &stat);
            break;
        } else if (flag_image) {
            //get new image
            CImg<double> image(IMAGE_WIDTH, IMAGE_HEIGHT, 1, 1, 0);
            MPI_Recv(image._data, IMAGE_WIDTH*IMAGE_HEIGHT, MPI_DOUBLE, 0, TAG_DATA, MPI_COMM_WORLD, &stat);
            //process image
            image = lucy_run(image, psf);
            //send image
            printf("slave sending the image\n");
            MPI_Send(image._data, image._height*image._width, MPI_DOUBLE, 0, TAG_DATA, MPI_COMM_WORLD);
        }
    }
}

int main(int argc, char *argv[]) {
    int numprocs;
    int myid;

    MPI_Status stat;

    MPI_Init(&argc, &argv); // all MPI programs start with MPI_Init; all 'N'
    //processes exist thereafter
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs); // find out how big the SPMD
    //world is
    MPI_Comm_rank(MPI_COMM_WORLD, &myid); // and this processes' rank is

    if (myid == 0) {
        master_program_main(stat, numprocs);
    } else {
        slave_program_main(stat);
    }

    printf("process %i finished\n",myid);
    MPI_Finalize(); // MPI Programs end with MPI Finalize; this is a weak
    //synchronization point
    return 0;
}