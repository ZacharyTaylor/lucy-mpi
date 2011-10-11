/**
 * The lucy richardson image deblurring algorithem
 */

#ifndef LUCY_H
#define	LUCY_H

#include "CImg.h"

//number of iterations to run
#define ITERATIONS 10

using namespace cimg_library;

/**
 * Runs the lucy-richardson algorithem on the image
 * @param base_img the orignal image
 * @param psf the point spread function of the image
 * @return the deblurred image
 */
CImg<double> lucy_run(CImg<double> base_img, CImg<double> psf);

#endif	/* LUCY_H */

