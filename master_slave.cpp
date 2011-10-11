/**
 * The master and slave functions to run
 */
#include "master_slave.h"

/**
 * Checks if there are still images to be sent and if there are sends the next
 * one
 * @param upto the image that is next to be processed
 * @param processing array of which processes are working on images
 * @param i the processes number
 * @param files a list of all the file loctaions of images
 */
void send_next_image(unsigned int *upto, unsigned int *processing, int i, vector<string> files) {
    //if there are still images to process
    if (*upto != files.size()) {
        processing[i] = (*upto)++;

        //get the next image and send it for processing
        cimg_library::CImg<double> image = get_image(files, processing[i]);
        printf("sent %s to slave %i\n", files[processing[i]].c_str(), i);
        mpi_send_image(image, TAG_DATA, i);
    }
}

/**
 * The master loads the images from ./Images and sends them to the slaves for
 * processing. It then receives the images back and saves them to ./Output
 * @param stat the MPI status
 * @param numprocs the number of processors being run
 */
void master_program_main(MPI_Status stat, int numprocs) {

    //a clock used to get an indication of how long the program spends in the
    //serial sections
    double start_s = (clock() / (double) CLOCKS_PER_SEC);
    double elapsed_s = 0;

    //if the processing is finished
    bool all_done = false;
    //stores which image each process is working on
    unsigned int processing[numprocs];
    //the next image to be loaded in
    unsigned int upto = 0;
    //the last image to be completed
    unsigned int doneto = 0;

    //gets list of all images to process
    vector<string> files = vector<string > ();
    init_images(&files);

    //gets the psf
    cimg_library::CImg<double> psf = get_psf();
    printf("loaded the psf\n");

    //sends the psf to all the slaves
    for (int i = 1; i < numprocs; i++) {
        mpi_send_image(psf, TAG_PSF, i);
    }

    //start by sending an image to each slave
    for (int i = 1; i < numprocs; i++) {
        send_next_image(&upto, processing, i, files);
    }

    //the end of this serial section of code
    elapsed_s = (clock() / (double) CLOCKS_PER_SEC) - start_s;

    printf("master in while loop\n");

    //while the process has not finsihed
    while (!all_done) {
        //for each process
        for (int i = 1; i < numprocs; i++) {

            //probe asyncronosly to see if image is being sent back by slave
            int flag_image = false;
            MPI_Iprobe(i, TAG_DATA, MPI_COMM_WORLD, &flag_image, &stat);

            //if an image is being sent
            if (flag_image == true) {

                //start serial timer again
                start_s = (clock() / (double) CLOCKS_PER_SEC);

                //get output image
                cimg_library::CImg<double> output = mpi_receive_image(TAG_DATA, i, &stat);
                printf("master got %s back from slave %i and saved as output-%i\n", files[processing[i]].c_str(), i, processing[i]);

                //save it
                save_image(output, files[processing[i]]);

                //mark as finsihed
                doneto++;
                printf("up to %i, done to %i  of %i\n", upto, doneto, files.size());

                //send next image to be processed
                send_next_image(&upto, processing, i, files);

                //if all images have been processed
                if (doneto == files.size()) {
                    //end the loop
                    all_done = true;

                    //let all the slaves know they can end
                    for (int i = 1; i < numprocs; i++) {
                        int end = 0;
                        MPI_Send(&end, 1, MPI_INT, i, TAG_END, MPI_COMM_WORLD);
                    }
                }
                //end of serial section of code
                elapsed_s += (clock() / (double) CLOCKS_PER_SEC) - start_s;
            }
        }
    }
    printf("serial run time = %g seconds\n", elapsed_s);
}

/**
 * Slave process takes in images from master processes them and sends them back
 * @param stat the status of the slave
 */
void slave_program_main(MPI_Status stat) {

    //gets the psf
    CImg<double> psf = mpi_receive_image(TAG_PSF, 0, &stat);
    //timer for parrallel section timing how long slave lives for
    double elapsed_p = 0;

    //loop till master says stop
    while (true) {
        printf("slave starting\n");

        //new image to process
        int flag_image = false;
        //all done time to stop flag
        int flag_end = false;

        //loop till an instruction from master received
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
        }
        //else grab image and run lucy richardson
        else if (flag_image) {

            //get new image
            CImg<double> image = mpi_receive_image(TAG_DATA, 0, &stat);

            //process image recording time spent processing
            double start_p = (clock() / (double) CLOCKS_PER_SEC);
            image = lucy_run(image, psf);
            elapsed_p += (clock() / (double) CLOCKS_PER_SEC) - start_p;

            //send image
            printf("slave sending the image\n");
            mpi_send_image(image, TAG_DATA, 0);
        }
    }
    printf("parallel run time = %f seconds\n", elapsed_p);
}
