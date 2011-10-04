#include "send_receive.h"

void mpi_send_image(CImg<double> img, int tag, int to){
    int img_size[3];
    img_size[HEIGHT] = img._height;
    img_size[WIDTH] = img._height;
    img_size[SPECTRUM] = img._spectrum;

    MPI_Send(&img_size, 3, MPI_INT, to, TAG_IMG_SIZE, MPI_COMM_WORLD);
    MPI_Send(img._data, img_size[HEIGHT]*img_size[WIDTH]*img_size[SPECTRUM], MPI_DOUBLE, to, tag, MPI_COMM_WORLD);
}

CImg<double> mpi_receive_image(int tag, int from, MPI_Status *stat){
    int img_size[3];
    MPI_Recv(&img_size, 3, MPI_INT, from, TAG_IMG_SIZE, MPI_COMM_WORLD, stat);
    
    CImg<double> img(img_size[WIDTH], img_size[HEIGHT], 1, img_size[SPECTRUM], 0);

    MPI_Recv(img._data, img_size[HEIGHT]*img_size[WIDTH]*img_size[SPECTRUM], MPI_DOUBLE, from, tag, MPI_COMM_WORLD, stat);

    return img;
}
