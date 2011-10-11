#ifndef SEND_RECEIVE_H
#define	SEND_RECEIVE_H

#include <mpi.h>
#include "CImg.h"

using namespace cimg_library;

#define HEIGHT 0
#define WIDTH 1
#define SPECTRUM 2

#define TAG_PSF 0
#define TAG_DATA 1
#define TAG_END 2
#define TAG_IMG_SIZE 3

void mpi_send_image(CImg<double> img, int tag, int to);

CImg<double> mpi_receive_image(int tag, int from, MPI_Status *stat);

#endif	/* SEND_RECEIVE_H */

