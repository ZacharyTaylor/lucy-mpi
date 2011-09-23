#ifndef MPI_BEHAVIOUR_H
#define	MPI_BEHAVIOUR_H

#include <mpi.h>
#include <CImg.h>
#include <stdio.h>

#define IMAGE_WIDTH 1024
#define IMAGE_HEIGHT 1024

using namespace cimg_library;

void mpi_image_send(CImg<double> image, int numprocs, int tag);

CImg<double> mpi_image_receive(int tag, MPI_Status stat);

#endif	/* MPI_BEHAVIOUR_H */

