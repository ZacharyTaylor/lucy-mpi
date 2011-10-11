/**
 * The lucy richardson image deblurring algorithem
 */

#include "lucy.h"

/**
 * Runs the 10 itertaions of the Lucy-Richardson deconvolution. Written without
 * using the Cimg .convolute function and with a large amount of nested for
 * loops so that OpenMP could be used to easily split up the loops
 * @param base_img the base image
 * @param psf_img the psf
 * @return the deblurred image
 */
CImg<double> lucy_run(CImg<double> base_img, CImg<double> psf_img) {

    CImg<double> out_img(base_img._width, base_img._height, 1, base_img._spectrum, 0);
    CImg<double> int_img(base_img._width, base_img._height, 1, base_img._spectrum, 0);
    CImg<double> in_img(base_img._width, base_img._height, 1, base_img._spectrum, 0);

    psf_img.normalize();

    in_img = base_img;

    //loop of iterations
    for (unsigned int it = 0; it < ITERATIONS; it++) {
        //loop over x of image
#pragma omp parallel for
        for (unsigned int x_img = 0; x_img < in_img._width; x_img++) {
            //loop over y of image
#pragma omp parallel for
            for (unsigned int y_img = 0; y_img < in_img._height; y_img++) {
                //loop over color channels of image
                for (unsigned int c_img = 0; c_img < in_img._spectrum; c_img++) {
                    int_img(x_img, y_img, 0, c_img) = 0;
                    //loop over x of psf
#pragma omp parallel for
                    for (unsigned int x_psf = 0; x_psf < psf_img._width; x_psf++) {
                        //loop over y of psf
                        for (unsigned int y_psf = 0; y_psf < psf_img._height; y_psf++) {
                            //calculate x and y offset for convolution
                            int x_offset = x_img + x_psf - (psf_img._width / 2);
                            int y_offset = y_img + y_psf - (psf_img._height / 2);

                            //if inside bounds
                            if ((((unsigned int) x_offset) < in_img._width) &&
                                    (x_offset >= 0) &&
                                    (((unsigned int) y_offset) < in_img._height) &&
                                    (y_offset >= 0)) {

                                //performs first convolution
                                int_img(x_img, y_img, 0, c_img) += in_img(x_offset, y_offset, 0, c_img) * psf_img(x_psf, y_psf, 0, 0);
                            }
                        }
                    }
                    //devides original image by convoluted one
                    int_img(x_img, y_img, 0, c_img) = base_img(x_img, y_img, 0, c_img) / int_img(x_img, y_img, 0, c_img);
                }
            }
        }
        //loop over x of image
#pragma omp parallel for
        for (unsigned int x_img = 0; x_img < in_img._width; x_img++) {
            //loop over y of image
#pragma omp parallel for
            for (unsigned int y_img = 0; y_img < in_img._height; y_img++) {
                //loop over color channels of image
#pragma omp parallel for
                for (unsigned int c_img = 0; c_img < in_img._spectrum; c_img++) {
                    out_img(x_img, y_img, 0, c_img) = 0;
                    //loop over x of psf
                    for (unsigned int x_psf = 0; x_psf < psf_img._width; x_psf++) {
                        //loop over y of psf
                        for (unsigned int y_psf = 0; y_psf < psf_img._height; y_psf++) {
                            //calculate x and y offset for convolution
                            int x_offset = x_img + x_psf - (psf_img._width / 2);
                            int y_offset = y_img + y_psf - (psf_img._height / 2);

                            //if inside bounds
                            if ((((unsigned int) x_offset) < in_img._width) &&
                                    (x_offset >= 0) &&
                                    (((unsigned int) y_offset) < in_img._height) &&
                                    (y_offset >= 0)) {

                                //performs second convolution
                                out_img(x_img, y_img, 0, c_img) += int_img(x_offset, y_offset, 0, c_img) * psf_img(x_psf, y_psf, 0, 0);
                            }
                        }
                    }
                    //multiplies base image by convoluted image
                    out_img(x_img, y_img, 0, c_img) = in_img(x_img, y_img, 0, c_img) * out_img(x_img, y_img, 0, c_img);
                }
            }
        }
        //set output to input to rerun for iterating
        in_img = out_img;
    }
    //ensures all pixels are still in the 0 to 255 range
#pragma omp parallel for
    for (unsigned int x_img = 0; x_img < base_img._width; x_img++) {
        //loop over y of image
#pragma omp parallel for
        for (unsigned int y_img = 0; y_img < base_img._height; y_img++) {
            //loop over color channels of image
#pragma omp parallel for
            for (unsigned int c_img = 0; c_img < base_img._spectrum; c_img++) {
                //limit values to 0 to 255 range
                if (out_img(x_img, y_img, 0, c_img) > 255) {
                    out_img(x_img, y_img, 0, c_img) = 255;
                } else if (out_img(x_img, y_img, 0, c_img) < 0) {
                    out_img(x_img, y_img, 0, c_img) = 0;
                }
            }
        }
    }
    return out_img;
}
