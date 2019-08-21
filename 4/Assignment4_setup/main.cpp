#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

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
    int max_bounces;
    char* outputfilename;
    //./a4 -input scene03 sphere.txt -size 200 200 -output 3.bmp
    //trash 0.
    for( int argNum = 1; argNum < argc; ++argNum )
    {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
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

    for (int widthi = 0; widthi < width; widthi++) {
        //screen_w_pixel = (float(widthi) + 0.5 / float(width)) * 2 - 1;
        screen_w_pixel = (2.0f * float(widthi)) / (float(width) - 1) - 1;
        //calculate the item in screen space..
        for (int heighti = 0; heighti < height; heighti++) {
            screen_h_pixel = (2.0f * float(heighti)) / (float(height) - 1) - 1;
            Hit newhit = Hit();
            Ray newray = camera->generateRay(Vector2f(screen_w_pixel, screen_h_pixel)); //shoot a ray out.
            bool yes = all_items->intersect(newray, newhit, tmin); //loop through all objects and get the ray - hit combination.
            if (yes) { //yes.
                Vector3f color; //we need to store an rgb color.
                for (int k = 0; k < totallights; k++) {
                    //cout << k<< endl;
                    
                    Light* light = parser.getLight(k);
                    Vector3f direction;
                    Vector3f lightColor;
                    float distance;
                    light->getIllumination(newray.pointAtParameter(newhit.getT()), direction, lightColor, distance);
                    //the direction and illumination here are overwritten inside light
                    Vector3f temp = newhit.getMaterial()->Shade(newray, newhit, direction, lightColor); //call for a shadey boi, based off materials.
                    color += temp;
                }
                color += ambient * newhit.getMaterial()->getDiffuseColor(); //add ambient light.
                //cout << screen_w_pixel << "strick " << screen_h_pixel << endl;
                image.SetPixel(widthi, heighti, color);
            }
            else {
                //cout << screen_w_pixel << " no strik " << screen_h_pixel << endl;
                image.SetPixel(widthi, heighti, parser.getBackgroundColor());
            }
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

