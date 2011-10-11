/**
 *Holds functions for sending and receiving images using mpi
 */
#include "send_receive.h"

/**
 * Sends an abitraly sized CImg<double> image
 * @param img the image
 * @param tag the tag to send with the image
 * @param to which process the image is being sent to
 */
void mpi_send_image(CImg<double> img, int tag, int to){
    //create array holding image info
    int img_size[3];
    img_size[HEIGHT] = img._height;
    img_size[WIDTH] = img._height;
    img_size[SPECTRUM] = img._spectrum;

    //send image info
    MPI_Send(&img_size, 3, MPI_INT, to, TAG_IMG_SIZE, MPI_COMM_WORLD);
    //send image
    MPI_Send(img._data, img_size[HEIGHT]*img_size[WIDTH]*img_size[SPECTRUM], MPI_DOUBLE, to, tag, MPI_COMM_WORLD);
}

/**
 * Receive an arbitray CImg<double> image
 * @param tag the tag sent with the image
 * @param from where it was sent from
 * @param stat the MPI status
 * @return returns the image
 */
CImg<double> mpi_receive_image(int tag, int from, MPI_Status *stat){
    //first receive array to hold info about image
    int img_size[3];
    MPI_Recv(&img_size, 3, MPI_INT, from, TAG_IMG_SIZE, MPI_COMM_WORLD, stat);

    //then using info create image to hold received data
    CImg<double> img(img_size[WIDTH], img_size[HEIGHT], 1, img_size[SPECTRUM], 0);

    //then receive image
    MPI_Recv(img._data, img_size[HEIGHT]*img_size[WIDTH]*img_size[SPECTRUM], MPI_DOUBLE, from, tag, MPI_COMM_WORLD, stat);

    //and return it
    return img;
}
