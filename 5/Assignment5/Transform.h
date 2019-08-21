#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "Object3D.h"
///TODO implement this class
///So that the intersect function first transforms the ray
///Add more fields as necessary
class Transform: public Object3D
{
public: 
    Matrix4f m;
  Transform(){}
 Transform( const Matrix4f& m, Object3D* obj ):o(obj){
     this->m = m;
     this->o = obj;
  }
  ~Transform(){
  }
  virtual bool intersect( const Ray& r , Hit& h , float tmin){
      //this is because we need to translate back to the real world coordinates. i think
      Vector4f oldorigin = Vector4f(r.getOrigin(), 1);
      Vector4f olddirection= Vector4f(r.getDirection(), 0); //get transform
      Vector3f neworigin = (m.inverse() * oldorigin).xyz(); //move by inverse of matrix.
      Vector3f newdirect = (m.inverse() * olddirection).xyz();
      Ray transformedRay = Ray(neworigin, newdirect);

      bool ting = o->intersect(transformedRay, h, tmin);
      if (ting) {
          Vector3f untransformednormal = (m.inverse().transposed() * Vector4f(h.getNormal(), 0) ).xyz();
          h.set(h.getT(), h.getMaterial(), untransformednormal);
          return true;
      }
      return false;

  }

 protected:
  Object3D* o; //un-transformed object	
};

#endif //TRANSFORM_H
