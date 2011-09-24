#ifndef LUCY_H
#define	LUCY_H

#include "CImg.h"

#define ITERATIONS 10

using namespace cimg_library;

void lucy_run(CImg<double> base_img, CImg<double> f_psf);

#endif	/* LUCY_H */

