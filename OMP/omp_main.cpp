/*
"Hello World" Type MPI Test Program
 */
#include "../CImg/CImg.h"
#include <stdio.h>
#include <string.h>
#include <vector>

#include "lucy.h"
#include "files_io.h"

using namespace std;

int main(int argc, char *argv[]) {

    cimg_library::CImg<double> image;

    double start = (clock() / (double) CLOCKS_PER_SEC);
    double elapsed_s = 0;
    //get list of all images to process
    vector<string> files = vector<string > ();
    init_images(&files);

    //get the psf
    cimg_library::CImg<double> psf = get_psf();
    printf("loaded the psf\n");

    #pragma omp parallel for
    for(unsigned int i = 0; i < files.size(); i++) {

        //get the image
        #pragma omp critical
        {
            double start_s = (clock() / (double) CLOCKS_PER_SEC);
            image = get_image(files, i);
            elapsed_s += (clock() / (double) CLOCKS_PER_SEC) - start_s;
        }

        printf("started image %i\n",i);

        //process the image
        image = lucy_run(image, psf);

        printf("finished image %i\n",i);
        //save it
        #pragma omp critical
        {
            double start_s = (clock() / (double) CLOCKS_PER_SEC);
            save_image(image, files[i]);
            elapsed_s += (clock() / (double) CLOCKS_PER_SEC) - start_s;
        }
    }

    double elapsed = (clock() / (double) CLOCKS_PER_SEC) - start;
    printf("parallel run time = %f seconds\n", elapsed - elapsed_s);
    printf("serial run time = %f seconds\n", elapsed_s);
    printf("total run time = %f seconds\n", elapsed);

    return 0;
}
