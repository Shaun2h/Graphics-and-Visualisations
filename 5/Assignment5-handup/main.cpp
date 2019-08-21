#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include "RayTracer.h"
#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include <string.h>

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
    char* filename;
    int height;
    int width;
    int max_bounces = 4;
    char* outputfilename;
    //./a4 -input scene03 sphere.txt -size 200 200 -output 3.bmp
    //trash 0.
    for( int argNum = 1; argNum < argc; ++argNum )
    {
        //std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
        if (!strcmp(argv[argNum], "-input")) {
            filename = argv[argNum + 1];
            argNum += 1; // because it comes in sets of 2.
            //cout << filename << endl;
        }
        else {
            if (!strcmp(argv[argNum],"-size")) {
                height = std::atoi(argv[argNum + 1]);
                width = std::atoi(argv[argNum + 2]);
                argNum += 2; // 3 here. 200 200 etc,.
                //cout << height<< endl;
                //cout << width<< endl;
            }
            else {
                if (!strcmp(argv[argNum], "-output")) {
                    outputfilename = argv[argNum + 1];
                    argNum += 1;
                    //cout << outputfilename<< endl;
                }
                else {
                    if (!strcmp(argv[argNum], "-max_bounces")) {
                        max_bounces = std::atoi(argv[argNum + 1]);
                        argNum += 1;
                        //cout << outputfilename<< endl;
                    }
                }

            }
        }
        
    }
    Image image(width, height);
    //all arguments get.
    // First, parse the scene using SceneParser.
    SceneParser parser = SceneParser(filename); //use char* instead for some reason.
    Camera* camera = parser.getCamera(); //obtain cam
    Group* all_items = parser.getGroup(); //get all items in the scene.
    Vector3f ambient = parser.getAmbientLight();
    float inv_aspect = width / height;
    float screen_w_pixel; //origin in screen space, width value.
    float screen_h_pixel; //origin in screen space, height value
    float totallights = parser.getNumLights(); //get number of lights
    float tmin = camera->getTMin(); 
    RayTracer rayTracer=RayTracer(&parser,max_bounces);
    /*
    for (int widthi = 0; widthi < width; widthi++) {
        //screen_w_pixel = (float(widthi) + 0.5 / float(width)) * 2 - 1;
        screen_w_pixel = (2.0f * float(widthi)) / (float(width) - 1) - 1;
        //calculate the item in screen space..
        for (int heighti = 0; heighti < height; heighti++) {
            screen_h_pixel = (2.0f * float(heighti)) / (float(height) - 1) - 1;
            Hit newhit = Hit();
            Ray newray = camera->generateRay(Vector2f(screen_w_pixel, screen_h_pixel)); //shoot a ray out.
            Vector3f newpixelColor = rayTracer.traceRay(newray, camera->getTMin(), max_bounces, 1.0f, newhit);
            image.SetPixel(widthi, heighti, newpixelColor);
        }
    }
    */

    vector<vector<Vector3f>> pixels;
    for (int widthi = 0; widthi < width*3; widthi++) {
        //screen_w_pixel = (float(widthi) + 0.5 / float(width)) * 2 - 1;
        vector<Vector3f> latest_column;
        //calculate the item in screen space..
        for (int heighti = 0; heighti < height*3; heighti++) {
            float rnd1 = ((float)rand() / (float)RAND_MAX - 0.5f ) / (width*3);
            float rnd2 = ((float)rand() / (float)RAND_MAX - 0.5f) / (height*3);
            screen_w_pixel = (2.0f * float(widthi)) / (float(width*3) - 1) - 1 + rnd1;
            screen_h_pixel = (2.0f * float(heighti)) / (float(height * 3) - 1) - 1 + rnd2 ;
            Hit newhit = Hit();
            Ray newray = camera->generateRay(Vector2f(screen_w_pixel, screen_h_pixel)); //shoot a ray out.
            Vector3f newpixelColor = rayTracer.traceRay(newray, camera->getTMin(), max_bounces, 1.0f, newhit);
            //image.SetPixel(widthi, heighti, newpixelColor);
            latest_column.push_back(newpixelColor);
        }
        pixels.push_back(latest_column);
    }
    //cout << "touch" << endl;
    vector<vector<Vector3f>> gaussianWpixel;
    for (int i = 0; i < width * 3; i++) {
        vector<Vector3f> latest_column;
        for (int j = 0; j < height * 3; j++) {
            if (i == 0) {
                //cout << pixels[i][j] << endl;
                //cout << pixels[i+1][j] << endl;
                //cout << pixels[i + 2][j] << endl;
                latest_column.push_back(pixels[i][j] * 0.1201 +
                                       pixels[i][j] * 0.2339 +
                                       pixels[i][j] * 0.2931 +
                                       pixels[i+1][j] * 0.2339 +
                                       pixels[i+2][j] * 0.1201 );
            }
            else if (i == 1) {
                latest_column.push_back(pixels[i-1][j] * 0.1201 +
                    pixels[i-1][j] * 0.2339 +
                    pixels[i][j] * 0.2931 +
                    pixels[i + 1][j] * 0.2339 +
                    pixels[i + 2][j] * 0.1201);
            }
            else if (i == width * 3-2) {
                latest_column.push_back(pixels[i-2][j] * 0.1201 +
                    pixels[i-1][j] * 0.2339 +
                    pixels[i][j] * 0.2931 +
                    pixels[i + 1][j] * 0.2339 +
                    pixels[i + 1][j] * 0.1201);
            }
            else if (i == width * 3-1) {
                latest_column.push_back( pixels[i-2][j] * 0.1201 +
                    pixels[i-1][j] * 0.2339 +
                    pixels[i][j] * 0.2931 +
                    pixels[i][j] * 0.2339 +
                    pixels[i][j] * 0.1201);
            }
            else {
                latest_column.push_back(pixels[i-2][j] * 0.1201 +
                    pixels[i-1][j] * 0.2339 +
                    pixels[i][j] * 0.2931 +
                    pixels[i + 1][j] * 0.2339 +
                    pixels[i + 2][j] * 0.1201);
            }
        }
        gaussianWpixel.push_back(latest_column);
    }
    vector<vector<Vector3f>> gaussianHpixel;
    for (int i = 0; i < width * 3; i++) {
        vector<Vector3f> latest_column;
        for (int j = 0; j < height * 3; j++) {
            if (j == 0) {
                latest_column.push_back(gaussianWpixel[i][j] * 0.1201 +
                    gaussianWpixel[i][j] * 0.2339 +
                    gaussianWpixel[i][j] * 0.2931 +
                    gaussianWpixel[i][j+1] * 0.2339 +
                    gaussianWpixel[i][j+2] * 0.1201);
            }
            else if (j == 1) {
                latest_column.push_back(gaussianWpixel[i][j-1] * 0.1201 +
                    gaussianWpixel[i][j-1] * 0.2339 +
                    gaussianWpixel[i][j] * 0.2931 +
                    gaussianWpixel[i][j+1] * 0.2339 +
                    gaussianWpixel[i][j+2] * 0.1201);
            }
            else if (j == height * 3 - 2) {
                latest_column.push_back(gaussianWpixel[i][j-2] * 0.1201 +
                    gaussianWpixel[i][j-1] * 0.2339 +
                    gaussianWpixel[i][j] * 0.2931 +
                    gaussianWpixel[i][j+1] * 0.2339 +
                    gaussianWpixel[i][j+1] * 0.1201);
            }
            else if (j == height* 3-1) {
                latest_column.push_back(gaussianWpixel[i][j-2] * 0.1201 +
                    gaussianWpixel[i][j-1] * 0.2339 +
                    gaussianWpixel[i][j] * 0.2931 +
                    gaussianWpixel[i][j] * 0.2339 +
                    gaussianWpixel[i][j] * 0.1201);
            }
            else {
                latest_column.push_back(gaussianWpixel[i][j-2] * 0.1201 +
                    gaussianWpixel[i][j-1] * 0.2339 +
                    gaussianWpixel[i][j] * 0.2931 +
                    gaussianWpixel[i][j+1] * 0.2339 +
                    gaussianWpixel[i][j+2] * 0.1201);
            }
        }
        gaussianHpixel.push_back(latest_column);
    }
    //downsample
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Vector3f average = gaussianHpixel[i * 3][j * 3]   + gaussianHpixel[i * 3+1][j * 3]   + gaussianHpixel[i * 3+2][j * 3]  +
                gaussianHpixel[i * 3][j * 3+1] + gaussianHpixel[i * 3+1][j * 3+1] + gaussianHpixel[i * 3+2][j * 3+1]+
                gaussianHpixel[i * 3][j * 3+2] + gaussianHpixel[i * 3+1][j * 3+2] + gaussianHpixel[i * 3+2][j * 3+2];
            average= average/ 9.0f;
            image.SetPixel(i, j,average);
        }
    }




    image.SaveImage(outputfilename);
  
  // Then loop over each pixel in the image, shooting a ray
  // through that pixel and finding its intersection with
  // the scene.  Write the color at the intersection to that
  // pixel in your output image.



 
  ///TODO: below demonstrates how to use the provided Image class
  ///Should be removed when you start
  /*
  Vector3f pixelColor (1.0f,0,0);
  //width and height
  Image image( 10 , 15 );
  image.SetPixel( 5,5, pixelColor );
  image.SaveImage("demo.bmp");
  return 0;
  */
}

