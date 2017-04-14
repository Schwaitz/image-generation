//
//  image.h
//  Image-Generation
//
//  Created by Justin Schwaitzberg on 4/11/17.
//  Copyright © 2017 Justin Schwaitzberg. All rights reserved.
//

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <cassert>
#include <string>
#include <iostream>
#include "color.h"


// ====================================================================
// ====================================================================
// TEMPLATED IMAGE CLASS
//    can be saved and loaded from standard file formats:
//      .ppm    (when T == Color)
//      .pgm    (when T == int)

template <class T>
class Image {
public:

    Image() : width(0), height(0), data(NULL) {}
    ~Image() { delete [] data; }
    
    void Allocate(int w, int h) {
        width = w;
        height = h;
        delete [] data;
        if (width == 0 && height == 0) {
            data = NULL;
        } else {
            assert (width > 0 && height > 0);
            data = new T[width * height];
        }
    }
    

    
    int get_width() const { return width; }
    int get_height() const { return height; }
    T *get_data() const { return data; }
    
    const T& GetPixel(int x, int y) const {
        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);
        return data[y*width + x]; }
    

    void SetPixel(int x, int y, const T &value) {
        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);
        data[y * width + x] = value; }
    

    bool Load(const std::string &filename);
    bool Save(const std::string &filename) const;
    
private:
    const Image& operator=(const Image &image) { assert(0); }
    Image(const Image &image) { assert(0); }
    int width;
    int height;
    T *data;
};

#endif
