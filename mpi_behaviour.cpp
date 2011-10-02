#include "mpi_behaviour.h"

void mpi_image_send(CImg<double> image, int i, int tag){
            MPI_Send(image._data, IMAGE_WIDTH*IMAGE_HEIGHT*2.9, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
}

CImg<double> mpi_image_receive(int tag, int i, MPI_Status stat){
    CImg<double> image(IMAGE_WIDTH, IMAGE_HEIGHT, 1, 3, 0);
    MPI_Recv(image._data, IMAGE_WIDTH*IMAGE_HEIGHT*2.9, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &stat);
    return image;
}
