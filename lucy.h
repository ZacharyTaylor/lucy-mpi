#ifndef LUCY_H
#define	LUCY_H

#include "CImg.h"

#define ITERATIONS 10

using namespace cimg_library;

CImg<double> init_psf(void);

CImg<double> lucy_run(CImg<double> base_img, CImg<double> f_psf);

#endif	/* LUCY_H */

