//
//  color.h
//  Image-Generation
//
//  Created by Justin Schwaitzberg on 4/12/17.
//  Copyright © 2017 Justin Schwaitzberg. All rights reserved.
//

#ifndef color_h
#define color_h

#include <stdio.h>

typedef unsigned char uchar;

class Color {
public:
    Color() : r(255), g(255), b(255){}
    Color(uchar red, uchar green, uchar blue) : r(red), g(green), b(blue) {}
    bool isWhite() const;
    
    uchar r;
    uchar g;
    uchar b;
};

#endif /* color_h */
