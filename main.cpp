//
//  main.cpp
//  Image-Generation
//
//  Created by Justin Schwaitzberg on 4/11/17.
//  Copyright © 2017 Justin Schwaitzberg. All rights reserved.
//


#include <vector>
#include <iomanip>
#include <string>
#include <random>
#include <ctime>
#include "image.h"

using namespace std;

void generate_image(int, int, int, int, string);
int get_random(int, int);
void make_pass(int, int, int, Color**&);
void validate_arguments(int, char*[]);

bool verbose = false;

int width;
int height;
int spread;
int passes;
string output;
int lowest_color_value;
int highest_color_value;

int r;
int g;
int b;




int main(int argc, char* argv[]) {
    
    lowest_color_value = 0;
    highest_color_value = 255;
    
    
    
    
    width = 512;
    height = 512;
    spread = 5;
    passes = 1;
    r = get_random(lowest_color_value, highest_color_value);
    g = get_random(lowest_color_value, highest_color_value);
    b = get_random(lowest_color_value, highest_color_value);
    output = "";
    
    validate_arguments(argc, argv);
    
    if(output == ""){
        output = "images/" + to_string(width) + "x" + to_string(height) + "_s=" + to_string(spread) + "_p=" + to_string(passes) + ".ppm";
    }
    

    
//    if(argc > 6){
//        if(strcmp(argv[6], "v") == 0){
//            verbose = true;
//        }
//    }
    
    srand((int) time(NULL));
    
    
    cout << endl << "----------------------------------------------------" << endl;
    printf("    Generating %dx%d image with a spread of %d\n", width, height, spread);
    cout << "----------------------------------------------------" << endl;
    
    generate_image(width, height, spread, passes, output);
    
    
    
}


void generate_image(int w, int h, int spread, int passes, string output){
    
    Color s(r, g, b);
    
    int m, m_r, m_g, m_b;
    Color o;
    Color **pixel_array = new Color*[w];
    
    
    for(int i = 0; i < w; i++){
        pixel_array[i] = new Color[h];
    }
    
    
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            
            
            if(y == 0 && x == 0){
                if(verbose == true){
                    printf("First Pass: Setting pixel (%d, %d) to RGB(%d, %d, %d)\n", x, y, s.r, s.g, s.b);
                }
                pixel_array[x][y] = s;
            }
            
            else if(y == 0){
                m_r = get_random(-spread, spread);
                m_g = get_random(-spread, spread);
                m_b = get_random(-spread, spread);
                
                o = pixel_array[x - 1][y];
                Color n(o.r + m_r, o.g + m_g, o.b + m_b);
                if(verbose == true){
                    printf("First Pass: Setting pixel (%d, %d) to RGB(%d, %d, %d)\n", x, y, n.r, n.g, n.b);
                }
                pixel_array[x][y] = n;
            }
            
            else if(x == 0){
                m_r = get_random(-spread, spread);
                m_g = get_random(-spread, spread);
                m_b = get_random(-spread, spread);
                
                o = pixel_array[x][y - 1];
                Color n(o.r + m_r, o.g + m_g, o.b + m_b);
                pixel_array[x][y] = n;
            }
            
            else {
                Color up = pixel_array[x][y - 1];
                Color left = pixel_array[x - 1][y];
                Color diagonal = pixel_array[x - 1][y - 1];
                
                float avg_r = 0;
                float avg_g = 0;
                float avg_b = 0;
                
                m = get_random(-spread, spread); avg_r += up.r + m;
                m = get_random(-spread, spread); avg_r += left.r + m;
                m = get_random(-spread, spread); avg_r += diagonal.r + m;
                
                m = get_random(-spread, spread); avg_g += up.g + m;
                m = get_random(-spread, spread); avg_g += left.g + m;
                m = get_random(-spread, spread); avg_g += diagonal.g + m;
                
                m = get_random(-spread, spread); avg_b += up.b + m;
                m = get_random(-spread, spread); avg_b += left.b + m;
                m = get_random(-spread, spread); avg_b += diagonal.b + m;
                
                int int_r = round(avg_r / 3);
                if(int_r > highest_color_value){ int_r = highest_color_value; }
                if(int_r < lowest_color_value){ int_r = lowest_color_value; }
                
                int int_g = round(avg_g / 3);
                if(int_g > highest_color_value){ int_g = highest_color_value; }
                if(int_g < lowest_color_value){ int_g = lowest_color_value; }
                
                int int_b = round(avg_b / 3);
                if(int_b > highest_color_value){ int_b = highest_color_value; }
                if(int_b < lowest_color_value){ int_b = lowest_color_value; }
                
                
                Color n(int_r, int_g, int_b);
                if(verbose == true){
                    printf("First Pass: Setting pixel (%d, %d) to RGB(%d, %d, %d)\n", x, y, n.r, n.g, n.b);
                }
                pixel_array[x][y] = n;
                
            }
        }
    }
    
    
    for(int i = 1; i < passes + 1; i++){
        cout << '\r' << "Making pass #" << i << flush;
        make_pass(w, h, spread, pixel_array);
    }
    cout << endl;
   
    
    
    
    Image<Color> out;
    out.Allocate(w, h);
    
    
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            out.SetPixel(x, y, pixel_array[x][y]);
        }
    }
    
    for(int i = 0; i < w; i++){
        delete [] pixel_array[i];
    }
    
    delete [] pixel_array;
    
    out.Save(output);
}

void make_pass(int w, int h, int spread, Color **&pixel_array){
    
    int m;
    int lowest_color_value = 0;
    int highest_color_value = 255;
    
    vector<Color> surrounding;
    
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            
            
            if(y == 0 && x == 0){
                surrounding.push_back(pixel_array[x + 1][y]); // Right
                surrounding.push_back(pixel_array[x][y + 1]); // Down
                surrounding.push_back(pixel_array[x + 1][y + 1]); // Diagonal Right Down
                
                
                float avg_r = 0;
                float avg_g = 0;
                float avg_b = 0;
                
                for(Color c : surrounding){
                    m = get_random(-spread, spread); avg_r += c.r + m;
                    m = get_random(-spread, spread); avg_g += c.g + m;
                    m = get_random(-spread, spread); avg_b += c.b + m;
                }
                
                int int_r = round(avg_r / surrounding.size());
                if(int_r > highest_color_value){ int_r = highest_color_value; }
                if(int_r < lowest_color_value){ int_r = lowest_color_value; }
                
                int int_g = round(avg_g / surrounding.size());
                if(int_g > highest_color_value){ int_g = highest_color_value; }
                if(int_g < lowest_color_value){ int_g = lowest_color_value; }
                
                int int_b = round(avg_b / surrounding.size());
                if(int_b > highest_color_value){ int_b = highest_color_value; }
                if(int_b < lowest_color_value){ int_b = lowest_color_value; }
                
                
                Color n(int_r, int_g, int_b);
                
                if(verbose == true){
                    printf("Second Pass: Setting pixel (%d, %d) to RGB(%d, %d, %d)\n", x, y, n.r, n.g, n.b);
                }
                
                pixel_array[x][y] = n;
                
            }
            
            else if(y == 0){
                if(x != w - 1){
                    surrounding.push_back(pixel_array[x + 1][y]); // Right
                }
                surrounding.push_back(pixel_array[x - 1][y]); // Left
                surrounding.push_back(pixel_array[x][y + 1]); // Down
                if(x != w - 1){
                    surrounding.push_back(pixel_array[x + 1][y + 1]); // Diagonal Right Down
                }
                surrounding.push_back(pixel_array[x - 1][y + 1]); // Diagonal Left Down
                
                
                float avg_r = 0;
                float avg_g = 0;
                float avg_b = 0;
                
                for(Color c : surrounding){
                    m = get_random(-spread, spread); avg_r += c.r + m;
                    m = get_random(-spread, spread); avg_g += c.g + m;
                    m = get_random(-spread, spread); avg_b += c.b + m;
                }
                
                int int_r = round(avg_r / surrounding.size());
                if(int_r > highest_color_value){ int_r = highest_color_value; }
                if(int_r < lowest_color_value){ int_r = lowest_color_value; }
                
                int int_g = round(avg_g / surrounding.size());
                if(int_g > highest_color_value){ int_g = highest_color_value; }
                if(int_g < lowest_color_value){ int_g = lowest_color_value; }
                
                int int_b = round(avg_b / surrounding.size());
                if(int_b > highest_color_value){ int_b = highest_color_value; }
                if(int_b < lowest_color_value){ int_b = lowest_color_value; }
                
                
                Color n(int_r, int_g, int_b);
                
                if(verbose == true){
                    printf("Second Pass: Setting pixel (%d, %d) to RGB(%d, %d, %d)\n", x, y, n.r, n.g, n.b);
                }
                
                pixel_array[x][y] = n;
                
                
            }
            
            else if(x == 0){
                surrounding.push_back(pixel_array[x + 1][y]); // Right
                surrounding.push_back(pixel_array[x][y - 1]); // Up
                if(y != h - 1){
                    surrounding.push_back(pixel_array[x][y + 1]); // Down
                }
                surrounding.push_back(pixel_array[x + 1][y - 1]); // Diagonal Right Up
                if(y != h - 1){
                    surrounding.push_back(pixel_array[x + 1][y + 1]); // Diagonal Right Down
                }
                
                
                float avg_r = 0;
                float avg_g = 0;
                float avg_b = 0;
                
                for(Color c : surrounding){
                    m = get_random(-spread, spread); avg_r += c.r + m;
                    m = get_random(-spread, spread); avg_g += c.g + m;
                    m = get_random(-spread, spread); avg_b += c.b + m;
                }
                
                int int_r = round(avg_r / surrounding.size());
                if(int_r > highest_color_value){ int_r = highest_color_value; }
                if(int_r < lowest_color_value){ int_r = lowest_color_value; }
                
                int int_g = round(avg_g / surrounding.size());
                if(int_g > highest_color_value){ int_g = highest_color_value; }
                if(int_g < lowest_color_value){ int_g = lowest_color_value; }
                
                int int_b = round(avg_b / surrounding.size());
                if(int_b > highest_color_value){ int_b = highest_color_value; }
                if(int_b < lowest_color_value){ int_b = lowest_color_value; }
                
                
                Color n(int_r, int_g, int_b);
                
                if(verbose == true){
                    printf("Second Pass: Setting pixel (%d, %d) to RGB(%d, %d, %d)\n", x, y, n.r, n.g, n.b);
                }
                
                pixel_array[x][y] = n;
                
                
                
            }
            
            else if(y == h - 1 && x == w - 1){
                surrounding.push_back(pixel_array[x - 1][y]); // Left
                surrounding.push_back(pixel_array[x][y - 1]); // Up
                surrounding.push_back(pixel_array[x - 1][y - 1]); // Diagonal Left Up
                
                
                float avg_r = 0;
                float avg_g = 0;
                float avg_b = 0;
                
                for(Color c : surrounding){
                    m = get_random(-spread, spread); avg_r += c.r + m;
                    m = get_random(-spread, spread); avg_g += c.g + m;
                    m = get_random(-spread, spread); avg_b += c.b + m;
                }
                
                int int_r = round(avg_r / surrounding.size());
                if(int_r > highest_color_value){ int_r = highest_color_value; }
                if(int_r < lowest_color_value){ int_r = lowest_color_value; }
                
                int int_g = round(avg_g / surrounding.size());
                if(int_g > highest_color_value){ int_g = highest_color_value; }
                if(int_g < lowest_color_value){ int_g = lowest_color_value; }
                
                int int_b = round(avg_b / surrounding.size());
                if(int_b > highest_color_value){ int_b = highest_color_value; }
                if(int_b < lowest_color_value){ int_b = lowest_color_value; }
                
                
                Color n(int_r, int_g, int_b);
                
                if(verbose == true){
                    printf("Second Pass: Setting pixel (%d, %d) to RGB(%d, %d, %d)\n", x, y, n.r, n.g, n.b);
                }
                
                pixel_array[x][y] = n;
                
            }
            
            else if(y == h - 1){
                if(x != w - 1){
                    surrounding.push_back(pixel_array[x + 1][y]); // Right
                }
                surrounding.push_back(pixel_array[x - 1][y]); // Left
                surrounding.push_back(pixel_array[x][y - 1]); // Up
                if(x != w - 1){
                    surrounding.push_back(pixel_array[x + 1][y - 1]); // Diagonal Right Up
                }
                surrounding.push_back(pixel_array[x - 1][y - 1]); // Diagonal Left Up
                
                
                float avg_r = 0;
                float avg_g = 0;
                float avg_b = 0;
                
                for(Color c : surrounding){
                    m = get_random(-spread, spread); avg_r += c.r + m;
                    m = get_random(-spread, spread); avg_g += c.g + m;
                    m = get_random(-spread, spread); avg_b += c.b + m;
                }
                
                int int_r = round(avg_r / surrounding.size());
                if(int_r > highest_color_value){ int_r = highest_color_value; }
                if(int_r < lowest_color_value){ int_r = lowest_color_value; }
                
                int int_g = round(avg_g / surrounding.size());
                if(int_g > highest_color_value){ int_g = highest_color_value; }
                if(int_g < lowest_color_value){ int_g = lowest_color_value; }
                
                int int_b = round(avg_b / surrounding.size());
                if(int_b > highest_color_value){ int_b = highest_color_value; }
                if(int_b < lowest_color_value){ int_b = lowest_color_value; }
                
                
                Color n(int_r, int_g, int_b);
                
                if(verbose == true){
                    printf("Second Pass: Setting pixel (%d, %d) to RGB(%d, %d, %d)\n", x, y, n.r, n.g, n.b);
                }
                
                pixel_array[x][y] = n;
                
                
                
            }
            
            else if(x == w - 1){
                surrounding.push_back(pixel_array[x - 1][y]); // Left
                surrounding.push_back(pixel_array[x][y - 1]); // Up
                if(y != h - 1){
                    surrounding.push_back(pixel_array[x][y + 1]); // Down
                }
                surrounding.push_back(pixel_array[x - 1][y - 1]); // Diagonal Left Up
                if(y != h - 1){
                    surrounding.push_back(pixel_array[x - 1][y + 1]); // Diagonal Left Down
                }
                
                
                float avg_r = 0;
                float avg_g = 0;
                float avg_b = 0;
                
                for(Color c : surrounding){
                    m = get_random(-spread, spread); avg_r += c.r + m;
                    m = get_random(-spread, spread); avg_g += c.g + m;
                    m = get_random(-spread, spread); avg_b += c.b + m;
                }
                
                int int_r = round(avg_r / surrounding.size());
                if(int_r > highest_color_value){ int_r = highest_color_value; }
                if(int_r < lowest_color_value){ int_r = lowest_color_value; }
                
                int int_g = round(avg_g / surrounding.size());
                if(int_g > highest_color_value){ int_g = highest_color_value; }
                if(int_g < lowest_color_value){ int_g = lowest_color_value; }
                
                int int_b = round(avg_b / surrounding.size());
                if(int_b > highest_color_value){ int_b = highest_color_value; }
                if(int_b < lowest_color_value){ int_b = lowest_color_value; }
                
                
                Color n(int_r, int_g, int_b);
                
                if(verbose == true){
                    printf("Second Pass: Setting pixel (%d, %d) to RGB(%d, %d, %d)\n", x, y, n.r, n.g, n.b);
                }
                
                pixel_array[x][y] = n;
                
                
            }
            
            else {
                surrounding.push_back(pixel_array[x + 1][y]); // Right
                surrounding.push_back(pixel_array[x - 1][y]); // Left
                
                surrounding.push_back(pixel_array[x][y - 1]); // Up
                surrounding.push_back(pixel_array[x][y + 1]); // Down
                
                surrounding.push_back(pixel_array[x + 1][y - 1]); // Diagonal Right Up
                
                surrounding.push_back(pixel_array[x - 1][y - 1]); // Diagonal Left Up
                surrounding.push_back(pixel_array[x - 1][y + 1]); // Diagonal Left Down
                
                surrounding.push_back(pixel_array[x + 1][y + 1]); // Diagonal Right Down
                
                
                
                float avg_r = 0;
                float avg_g = 0;
                float avg_b = 0;
                
                for(Color c : surrounding){
                    m = get_random(-spread, spread); avg_r += c.r + m;
                    m = get_random(-spread, spread); avg_g += c.g + m;
                    m = get_random(-spread, spread); avg_b += c.b + m;
                }
                
                int int_r = round(avg_r / surrounding.size());
                if(int_r > highest_color_value){ int_r = highest_color_value; }
                if(int_r < lowest_color_value){ int_r = lowest_color_value; }
                
                int int_g = round(avg_g / surrounding.size());
                if(int_g > highest_color_value){ int_g = highest_color_value; }
                if(int_g < lowest_color_value){ int_g = lowest_color_value; }
                
                int int_b = round(avg_b / surrounding.size());
                if(int_b > highest_color_value){ int_b = highest_color_value; }
                if(int_b < lowest_color_value){ int_b = lowest_color_value; }
                
                
                Color n(int_r, int_g, int_b);
                
                if(verbose == true){
                    printf("Second Pass: Setting pixel (%d, %d) to RGB(%d, %d, %d)\n", x, y, n.r, n.g, n.b);
                }
                
                pixel_array[x][y] = n;
                
            }
            
            
            surrounding.clear();
            surrounding.shrink_to_fit();
        }
    }

    
}


int get_random(int low, int high){
    return low + (rand() % (int)(high - low + 1));
}




void validate_arguments(int argc, char* argv[]){
    
    if(argc == 1){
        return;
    }
    
    for(int i = 1; i < argc - 1; i += 2){
        if(strcmp(argv[i], "-w") == 0){ width = stoi(argv[i + 1]); continue; }
        if(strcmp(argv[i], "-h") == 0){ height = stoi(argv[i + 1]); continue; }
        if(strcmp(argv[i], "-s") == 0){ spread = stoi(argv[i + 1]); continue; }
        if(strcmp(argv[i], "-p") == 0){ passes = stoi(argv[i + 1]); continue; }
        if(strcmp(argv[i], "-r") == 0){ r = stoi(argv[i + 1]); continue; }
        if(strcmp(argv[i], "-g") == 0){ g = stoi(argv[i + 1]); continue; }
        if(strcmp(argv[i], "-b") == 0){ b = stoi(argv[i + 1]); continue; }
        if(strcmp(argv[i], "-o") == 0){ output = argv[i + 1]; output = "images/" + output; continue; }
    }
    
    
}

//int get_random(int low, int high){
//    random_device rd;
//    mt19937 gen(rd());
//    uniform_int_distribution<> dis_n(low, high);
//    int rand = dis_n(gen);
//
//    return rand;
//}







