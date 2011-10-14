/**
 * The master and slave functions to run
 */

#ifndef MASTER_SLAVE_H
#define	MASTER_SLAVE_H

//includes
#include <mpi.h>
#include "../CImg/CImg.h"
#include <stdio.h>
#include <string.h>
#include <vector>

#include "lucy.h"
#include "files_io.h"
#include "send_receive.h"

using namespace std;

/**
 * Checks if there are still images to be sent and if there are sends the next
 * one
 * @param upto the image that is next to be processed
 * @param processing array of which processes are working on images
 * @param i the processes number
 * @param files a list of all the file loctaions of images
 */
void send_next_image(unsigned int *upto, unsigned int *processing, int i, vector<string> files);

/**
 * The master loads the images from ./Images and sends them to the slaves for
 * processing. It then receives the images back and saves them to ./Output
 * @param stat the MPI status
 * @param numprocs the number of processors being run
 */
void master_program_main(MPI_Status stat, int numprocs);

/**
 * Slave process takes in images from master processes them and sends them back
 * @param stat the status of the slave
 */
void slave_program_main(MPI_Status stat);

#endif	/* MASTER_SLAVE_H */
