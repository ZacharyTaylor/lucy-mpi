/**
 *Holds functions for sending and receiving images using mpi
 */

#ifndef SEND_RECEIVE_H
#define	SEND_RECEIVE_H

//includes
#include <mpi.h>
#include "CImg.h"

using namespace cimg_library;

//used to make array of image properties
#define HEIGHT 0
#define WIDTH 1
#define SPECTRUM 2

//tags used in sending images
#define TAG_PSF 0
#define TAG_DATA 1
#define TAG_END 2
#define TAG_IMG_SIZE 3

/**
 * Sends an abitraly sized CImg<double> image
 * @param img the image
 * @param tag the tag to send with the image
 * @param to which process the image is being sent to
 */
void mpi_send_image(CImg<double> img, int tag, int to);

/**
 * Receive an arbitray CImg<double> image
 * @param tag the tag sent with the image
 * @param from where it was sent from
 * @param stat the MPI status
 * @return returns the image
 */
CImg<double> mpi_receive_image(int tag, int from, MPI_Status *stat);

#endif	/* SEND_RECEIVE_H */

