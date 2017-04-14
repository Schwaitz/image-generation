//
//  color.cpp
//  Image-Generation
//
//  Created by Justin Schwaitzberg on 4/12/17.
//  Copyright © 2017 Justin Schwaitzberg. All rights reserved.
//

#include "color.h"

bool Color::isWhite() const {
    return r == 255 && g == 255 && b == 255;
}
