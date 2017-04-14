//
//  image.cpp
//  Image-Generation
//
//  Created by Justin Schwaitzberg on 4/11/17.
//  Copyright © 2017 Justin Schwaitzberg. All rights reserved.
//

#include <cstring>
#include <cstdio>
#include "image.h"

using namespace std;

// ====================================================================
// EXPLICIT SPECIALIZATIONS for Color images (.ppm)
// ====================================================================
template <>
bool Image<Color>::Save(const string &filename) const {
    int len = (int) filename.length();
    if (!(len > 4 && filename.substr(len-4) == string(".ppm"))) {
        cerr << "ERROR: This is not a PPM filename: " << filename << endl;
        return false;
    }
    FILE *file = fopen(filename.c_str(), "wb");
    if (file == NULL) {
        cerr << "Unable to open " << filename << " for writing\n";
        return false;
    }
    
    // misc header information
    fprintf (file, "P6\n");
    fprintf (file, "%d %d\n", width,height);
    fprintf (file, "255\n");
    
    // the data
    // flip y so that (0,0) is bottom left corner
    for (int y = height - 1; y >= 0; y--) {
        for (int x=0; x < width; x++) {
            Color v = GetPixel(x,y);
            fputc ((uchar)(v.r),file);
            fputc ((uchar)(v.g),file);
            fputc ((uchar)(v.b),file);
        }
    }
    fclose(file);
    return true;
}

template <>
bool Image<Color>::Load(const string &filename) {
    int len = (int) filename.length();
    if (!(len > 4 && filename.substr(len-4) == string(".ppm"))) {
        cerr << "ERROR: This is not a PPM filename: " << filename << endl;
        return false;
    }
    FILE *file = fopen(filename.c_str(),"rb");
    if (file == NULL) {
        cerr << "Unable to open " << filename << " for reading\n";
        return false;
    }
    
    char tmp[100];
    fgets(tmp,100,file);
    assert (strstr(tmp,"P6"));
    fgets(tmp,100,file);
    while (tmp[0] == '#') { fgets(tmp,100,file); }
    sscanf(tmp,"%d %d",&width,&height);
    fgets(tmp,100,file);
    assert (strstr(tmp,"255"));
    
    delete [] data;
    data = new Color[height * width];
    
    
    // Flip y so that (0,0) is bottom left corner
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            Color c;
            c.r = fgetc(file);
            c.g = fgetc(file);
            c.b = fgetc(file);
            SetPixel(x,y,c);
        }
    }
    fclose(file);
    return true;
}






// ====================================================================
// EXPLICIT SPECIALIZATIONS for greyscale images (.pbm)
// ====================================================================
template <>
bool Image<int>::Save(const string &filename) const {
    int len = (int) filename.length();
    if (!(len > 4 && filename.substr(len-4) == string(".pgm"))) {
        cerr << "ERROR: This is not a PPM filename: " << filename << endl;
        return false;
    }
    FILE *file = fopen(filename.c_str(), "wb");
    if (file == NULL) {
        cerr << "Unable to open " << filename << " for writing\n";
        return false;
    }
    
    // misc header information
    fprintf (file, "P5\n");
    fprintf (file, "%d %d\n", width,height);
    fprintf (file, "255\n");
    
    // Flip y so that (0,0) is bottom left corner
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            int v = GetPixel(x,y);
            fputc ((uchar)(v),file);
        }
    }
    fclose(file);
    return true;
}

template <>
bool Image<int>::Load(const string &filename) {
    int len = (int) filename.length();
    if (!(len > 4 &&
          (filename.substr(len-4) == string(".pgm") ||
           filename.substr(len-4) == string(".pbm")))) {
              cerr << "ERROR: This is not a PBM filename: " << filename << endl;
              return false;
          }
    FILE *file = fopen(filename.c_str(),"rb");
    if (file == NULL) {
        cerr << "Unable to open " << filename << " for reading\n";
        return false;
    }
    
    // misc header information
    char tmp[100];
    fgets(tmp,100,file);
    assert (strstr(tmp,"P5"));
    fgets(tmp,100,file);
    while (tmp[0] == '#') { fgets(tmp,100,file); }
    int read = sscanf(tmp,"%d %d",&width,&height);
    if (read == 1) {
        fgets(tmp, 100, file);
        while (tmp[0] == '#') { fgets(tmp, 100, file); }
        sscanf(tmp,"%d",&height);
    }
    fgets(tmp, 100, file);
    assert (strstr(tmp, "255"));
    
    // the data
    delete [] data;
    data = new int[height*width];
    // flip y so that (0,0) is bottom left corner
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            int v = fgetc(file);
            SetPixel(x,y,v);
        }
    }
    fclose(file);
    return true;
}

// ====================================================================
// ====================================================================
