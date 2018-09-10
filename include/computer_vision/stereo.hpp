/*

PICCANTE
The hottest HDR imaging library!
http://vcg.isti.cnr.it/piccante

Copyright (C) 2014
Visual Computing Laboratory - ISTI CNR
http://vcg.isti.cnr.it
First author: Francesco Banterle

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

#ifndef PIC_COMPUTER_VISION_STEREO_HPP
#define PIC_COMPUTER_VISION_STEREO_HPP

#include <vector>
#include <random>
#include <stdlib.h>

#include "../base.hpp"

#include "../image.hpp"

#include "../filtering/filter_disparity.hpp"

#include "../util/math.hpp"

namespace pic {

/**
 * @brief checkDisparity
 * @param disp_left
 * @param disp_right
 * @param threshold
 */
PIC_INLINE void checkDisparity(Image *disp_left, Image *disp_right, int threshold = 8)
{
    for(int i = 0; i < disp_left->height; i++) {

        for(int j = 0; j < disp_left->width; j++) {

            float *dL = (*disp_left)(j, i);
            float *dR = (*disp_right)(j, i);

            int d_L = int(dL[0]);
            int d_R = int(dR[0]);

            if(std::abs(d_L - d_R) > threshold) {
                dL[0] = 0.0f;
                dL[1] = -1.0f;

                dR[0] = 0.0f;
                dR[1] = -1.0f;
            }
        }
    }
}

/**
 * @brief estimateStereo
 * @param img_left
 * @param img_right
 * @param max_disparity
 * @param disparity_cross_check
 * @param disp_left
 * @param disp_right
 */
PIC_INLINE void estimateStereo(Image *img_left, Image *img_right,
                               int max_disparity, int disparity_cross_check,
                               Image *disp_left, Image *disp_right)
{
    if(img_left  == NULL || img_right  == NULL ||
       disp_left == NULL || disp_right == NULL) {
        return;
    }

    if(max_disparity < 0) {
        max_disparity = MIN(img_left->width, img_right->width) >> 1;
    }

    if(disparity_cross_check < 0) {
        disparity_cross_check = 16;
    }

    pic::FilterDisparity fd(240, 5);

    disp_left  = fd.ProcessP(pic::Double(img_left, img_right), disp_left);
    disp_right = fd.ProcessP(pic::Double(img_right, img_left), disp_right);

    checkDisparity(disp_left, disp_right, disparity_cross_check);
    checkDisparity(disp_right, disp_left, disparity_cross_check);
}

} // end namespace pic

#endif // PIC_COMPUTER_VISION_STEREO_HPP
