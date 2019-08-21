#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <ctime>
#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include <string.h>
#include "RayTracer.h"
#include "Group.h"
#include "Hit.h"
#include "Ray.h"

using namespace std;

float clampedDepth ( float depthInput, float depthMin , float depthMax);

#include "bitmap_image.hpp"
int main( int argc, char* argv[] )
{
    // Fill in your implementation here.
    // This loop loops over each of the input arguments.
    // argNum is initialized to 1 because the first
    // "argument" provided to the program is actually the
    // name of the executable (in our case, "a4").
    
    // example: ./a4 -input scene01 plane.txt -size 200 200 -output 1.bmp
//    if (argc <= 2){
//        std::cout << "Please enter: -input <scene.txt> -size <width> <height> -output <image.png> -bounces <max_bounces> -shadows -jittered -filter" << std::endl;
//        return 1;
//    }
//
    srand(time(NULL));
    char* input;
    char* output_Filename;
    int bounce;
    int width;
    int height;
    bool shadow = false;
    bool jitter = false;
    bool filter = false;
    
    for( int argNum = 1; argNum < argc; ++argNum )
    {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }
    
    for( int argNum = 1; argNum < argc; ++argNum )
    {
        if(!strcmp(argv[argNum], "-input")){
            input = argv[argNum+1];
            argNum+=1;
        }
        else if(!strcmp(argv[argNum], "-size")){
            width = atoi(argv[argNum+1]);
            height = atoi(argv[argNum+2]);
            argNum+=2;
        }
        else if(!strcmp(argv[argNum], "-output")){
            output_Filename = argv[argNum+1];
            argNum+=1;
        }
        else if(!strcmp(argv[argNum], "-bounces")){
            bounce = atoi(argv[argNum+1]);
            argNum+=1;
        }
        else if(!strcmp(argv[argNum], "-shadows")){
            shadow = true;
        }
        else if(!strcmp(argv[argNum], "-jittered")){
            jitter = true;
        }
        else if(!strcmp(argv[argNum], "-filter")){
            filter = true;
        }
        else{
            std::cout << "Please enter: -input <scene.txt> -size <width> <height> -output <image.png> -bounces <max_bounces> -shadows -jittered -filter" << std::endl;
            return 1;
        }
    }
    
    
    // First, parse the scene using SceneParser.
    SceneParser *scene = new SceneParser(input);
    Camera* camera = scene->getCamera();
    Group* group = scene->getGroup();
    Vector3f backgroundColor = scene->getBackgroundColor(Vector3f(0,0,0));
    RayTracer rayTracer = RayTracer(scene,bounce);
    
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.
    Image image(width, height);
    
    if (!jitter){
        for (int i = 0; i < width; i++){
            for (int j = 0; j < height; j++){
                float x = (2.0f * float(i)) / (width-1) -1;
                float y = (2.0f * float(j)) / (height-1) -1;
                Vector2f xy_coord = Vector2f(x,y);
                Ray ray = camera->generateRay(xy_coord);
                Hit hit = Hit();
                
                Vector3f pixelColor =rayTracer.traceRay(ray,camera->getTMin(), 0, 1.0f, hit);
                image.SetPixel(i, j, pixelColor);
                
            }
        }
    }
    
    else{
        // 1. render multiple samples per pixel
        int widthJitter = width*3;
        int heightJitter = height*3;
        vector<Vector3f> subPixelColor;
        
        for (int i = 0; i < widthJitter; i++){
            for (int j = 0; j < heightJitter; j++){
                float x = (2.0f * float(i)) / float((widthJitter-1)) -1;
                float y = (2.0f * float(j)) / float((heightJitter-1)) -1;
                
                // 2. jitter each subpixel sample location (add a small random displacement)
                float ri = (rand() % (5 - (-5) + 1) + (-5))/(widthJitter*10.0);
                float rj = (rand() % (5 - (-5) + 1) + (-5))/(widthJitter*10.0);
                Vector2f xy_coord = Vector2f(x+ri, y+rj);
                
                // 3. generate rays through the jittered subpixel location
                Ray ray = camera->generateRay(xy_coord);
                Hit hit = Hit();
                
                Vector3f color =rayTracer.traceRay(ray,camera->getTMin(), 0, 1.0f, hit);
                
                subPixelColor.push_back(color);
            }
        }
        
        // 4. Apply filter to reconstruct final image
        if (filter){
            const float K[] = {0.1201f, 0.2339f, 0.2931f, 0.2339f, 0.1201f};
            // allocate new image to do blurring to prevent bugs
            Image filterImg(widthJitter, heightJitter);
            vector<Vector3f> blurHorizontal;
            vector<Vector3f> blurVertical;
            
            // blur horizontal
            for (int i = 0; i < widthJitter; i++){
                for (int j = 0; j < heightJitter;j++){
                    // bottom row case
                    if (j == 0){
                        blurHorizontal.push_back(subPixelColor[i*heightJitter+j] * K[2]
                                                 + subPixelColor[i*heightJitter+j+1] * K[3]
                                                 + subPixelColor[i*heightJitter+j+2] * K[4]);
                    }
                    // second bottom row case
                    else if (j == 1){
                        blurHorizontal.push_back(subPixelColor[i*heightJitter+j-1] * K[1]
                                                 + subPixelColor[i*heightJitter+j] * K[2]
                                                 + subPixelColor[i*heightJitter+j+1] * K[3]
                                                 + subPixelColor[i*heightJitter+j+2] * K[4]);
                    }
                    // top row case
                    else if (j == heightJitter-1){
                        blurHorizontal.push_back(subPixelColor[i*heightJitter+j-2] * K[0]
                                                 + subPixelColor[i*heightJitter+j-1] * K[1]
                                                 + subPixelColor[i*heightJitter+j] * K[2]);
                    }
                    // second top row case
                    else if (j == heightJitter-2){
                        blurHorizontal.push_back(subPixelColor[i*heightJitter+j-2] * K[0]
                                                 + subPixelColor[i*heightJitter+j-1] * K[1]
                                                 + subPixelColor[i*heightJitter+j] * K[2]
                                                 + subPixelColor[i*heightJitter+j+1] * K[3]);
                    }
                    // normal case
                    else{
                        blurHorizontal.push_back(subPixelColor[i*heightJitter+j-2] * K[0]
                                                 + subPixelColor[i*heightJitter+j-1] * K[1]
                                                 + subPixelColor[i*heightJitter+j] * K[2]
                                                 + subPixelColor[i*heightJitter+j+1] * K[3]
                                                 + subPixelColor[i*heightJitter+j+2] * K[4]);
                    }
                }
            }
            
            // blur vertical
            for (int j=0; j<heightJitter; j++){
                for (int i=0; i<widthJitter; i++){
                    // right row case
                    if (i==0){
                        blurVertical.push_back(blurHorizontal[j*widthJitter+i] * K[2]
                                               + blurHorizontal[j*widthJitter+i+1] * K[3]
                                               + blurHorizontal[j*widthJitter+i+2] * K[4]);
                    }
                    // second right row case
                    else if (i==1){
                        blurVertical.push_back(blurHorizontal[j*widthJitter+i-1] * K[1]
                                               + blurHorizontal[j*widthJitter+i] * K[2]
                                               + blurHorizontal[j*widthJitter+i+1] * K[3]
                                               + blurHorizontal[j*widthJitter+i+2] * K[4]);
                    }
                    // left row case
                    else if (i == widthJitter-1){
                        blurVertical.push_back(blurHorizontal[j*widthJitter+i-2] * K[0]
                                               + blurHorizontal[j*widthJitter+i-1] * K[1]
                                               + blurHorizontal[j*widthJitter+i] * K[2]);
                    }
                    // second left row case
                    else if (i == widthJitter-2){
                        blurVertical.push_back(blurHorizontal[j*widthJitter+i-2] * K[0]
                                               + blurHorizontal[j*widthJitter+i-1] * K[1]
                                               + blurHorizontal[j*widthJitter+i] * K[2]
                                               + blurHorizontal[j*widthJitter+i+1] * K[3]);
                    }
                    // normal case
                    else{
                        blurVertical.push_back(blurHorizontal[j*widthJitter+i-2] * K[0]
                                               + blurHorizontal[j*widthJitter+i-1] * K[1]
                                               + blurHorizontal[j*widthJitter+i] * K[2]
                                               + blurHorizontal[j*widthJitter+i+1] * K[3]
                                               + blurHorizontal[j*widthJitter+i+2] * K[4]);
                    }
                }
            }
            
            // downsample
            for (int i = 0; i < width; i++){
                for (int j = 0; j < height; j++){
                    
                    Vector3f pixelColor =
                    (blurVertical[3*(j+heightJitter*i)+ 0]+
                     blurVertical[3*(j+heightJitter*i)+ 1]+
                     blurVertical[3*(j+heightJitter*i)+ 2]+
                     blurVertical[3*(j+heightJitter*i)+ heightJitter+0]+
                     blurVertical[3*(j+heightJitter*i)+ heightJitter+1]+
                     blurVertical[3*(j+heightJitter*i)+ heightJitter+2]+
                     blurVertical[3*(j+heightJitter*i)+ 2*heightJitter+0]+
                     blurVertical[3*(j+heightJitter*i)+ 2*heightJitter+1]+
                     blurVertical[3*(j+heightJitter*i)+ 2*heightJitter+2])/9.0f;
                    
                    image.SetPixel(i, j, pixelColor);
                }
            }
            
        }
        
        // 5. Obtain the pixel amount from the subpixels using weighted average
        else{
            for (int i = 0; i < width; i++){
                for (int j = 0; j < height; j++){

                    Vector3f pixelColor =
                        (subPixelColor[3*(j+heightJitter*i)+ 0]+
                        subPixelColor[3*(j+heightJitter*i)+ 1]+
                        subPixelColor[3*(j+heightJitter*i)+ 2]+
                        subPixelColor[3*(j+heightJitter*i)+ heightJitter+0]+
                        subPixelColor[3*(j+heightJitter*i)+ heightJitter+1]+
                        subPixelColor[3*(j+heightJitter*i)+ heightJitter+2]+
                        subPixelColor[3*(j+heightJitter*i)+ 2*heightJitter+0]+
                        subPixelColor[3*(j+heightJitter*i)+ 2*heightJitter+1]+
                        subPixelColor[3*(j+heightJitter*i)+ 2*heightJitter+2])/9.0f;
                    
                    image.SetPixel(i, j, pixelColor);
                }
            }
       }
    }
    
    image.SaveImage(output_Filename);
    
    return 0;
}

