#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"
///TODO:
///Implement Shade function that uses ambient, diffuse, specular and texture
class Material
{
public:
	
 Material( const Vector3f& d_color ,const Vector3f& s_color=Vector3f::ZERO, float s=0):
  diffuseColor( d_color),specularColor(s_color), shininess(s)
  {
        	
  }

  virtual ~Material()
    {

    }

  virtual Vector3f getDiffuseColor() const 
  { 
    return  diffuseColor;
  }
    

  Vector3f Shade(const Ray& ray, const Hit& hit,
      const Vector3f& dirToLight, const Vector3f& lightColor) {
      //diffuse shading
      Vector3f normal = hit.getNormal().normalized();
      Vector3f diffusecolor = diffuseColor; //get the diffuse color of the material. but looking at it after prints, it seems more like a.. diffuse parameter...
      //diffuse color is k_d.
      if (this->t.valid()) {
          diffusecolor = this->t(hit.texCoord[0], hit.texCoord[1]);
      }

      float irradiance = Vector3f::dot(normal, dirToLight); //okay i couldn't find anything else so i'm assuming it's irradiance that we should use.
      // assuming it's a point light.
      if (irradiance < 0) {
          irradiance = 0; //set it to 0
      }
      Vector3f diffusionlight;
      if (irradiance != 0) {
          diffusionlight = irradiance * Vector3f(diffusecolor[0] * lightColor[0], diffusecolor[1] * lightColor[1], diffusecolor[2] * lightColor[2]);
          // we do it for each rgb channel.
      }
      else {
          diffusionlight = Vector3f::ZERO;
      }
      //R =-L+ 2(l. n) n 
      //the second n is directional!
      //Vector3f reflected = -1.0f * ray.getDirection() + (2.0f * (Vector3f::dot(dirToLight, hit.getNormal())) * hit.getNormal()).normalized();
      Vector3f v = ray.getDirection() * -1.0f;
      Vector3f r = (2.0f * irradiance * hit.getNormal().normalized() - dirToLight).normalized();
      //Vector3f reflected = Vector3f::dot(ray.getDirection() * -1.0f, (2.0f * irradiance * hit.getNormal() - 1.0f * ray.getDirection()).normalized());
      float specconst = Vector3f::dot(v, r);
      //float specconst = Vector3f::dot(dirToLight, reflected);
      Vector3f speclight;
      if (specconst > 0) {
          specconst = pow(specconst, shininess);
          speclight = specconst* Vector3f(specularColor[0] * lightColor[0], specularColor[1] * lightColor[1], specularColor[2] * lightColor[2]); //apparently spec color is set before?

      }
      else {
          speclight = Vector3f::ZERO;
      }
      return diffusionlight;
      //return speclight;
      //return diffusionlight -speclight;
      //return diffusionlight + speclight;
  }
  
  void loadTexture(const char * filename){
    t.load(filename);
  }
 protected:
  Vector3f diffuseColor;
  Vector3f specularColor;
  float shininess;
  Texture t;
};



#endif // MATERIAL_H
