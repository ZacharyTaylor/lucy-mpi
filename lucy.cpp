#include "lucy.h"

CImg<double> lucy_run(CImg<double> base_img, CImg<double> f_psf){

    CImg<double> f_work;
    CImg<double> work_img = base_img;

    for (int i = 0; i < ITERATIONS; i++) {
        //take fourier transform
        f_work = work_img.get_FFT('z', false)[0];

        //matrix multiply with fourier of psf
        cimg_forXY(work_img, x, y) {
            f_work(x, y, 0, 0) = f_work(x, y, 0, 0) * f_psf(x, y, 0, 0);
        }

        //invert image
        work_img = f_work.get_FFT('z', true)[0];
        work_img = work_img.get_normalize(0.1, 1);

        //divide orignal by transformed image
        work_img = base_img.get_div(work_img);
        work_img = work_img.get_normalize(0, 255);
    }

    return work_img;
}


