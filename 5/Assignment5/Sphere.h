
#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

#include <iostream>
using namespace std;
///TODO:
// store center point and a radius
// sphere constructor given a center, radius, pointer to material instance
// sphere class implement virtual intersect method
    // virtual bool intersect(const Ray& r, Hit& h, float tmin)
    // with intersect routine, we look for the closest intersection along a Ray.
    // if t> tmin and t<value of intersection stored in Hit data structure. Hit updated.
    // if the new intersection is closer than previous, both t and material must be modified
    // intersection routine verifie that t>tmin, tmin not modified by the intersection routine.

///Implement functions and add more fields as necessary
class Sphere: public Object3D
{
public:
	Sphere(){ 
		//unit ball at the center
        this->center = Vector3f(0,0,0);
        this->radius = 1.0f;
    }

	Sphere( Vector3f center , float radius , Material* material ):Object3D(material){
        this->center = center;
        this->radius = radius;
	}
	
	~Sphere(){}
    
    
    
	virtual bool intersect( const Ray& r , Hit& h , float tmin){
        // translate sphere to be centered at origin
        Vector3f Ro = r.getOrigin() - this->center;
        Vector3f Rd = r.getDirection();
        float a = Vector3f::dot(Rd, Rd);
        float b = 2 * Vector3f::dot(Rd, Ro);
        float c = Vector3f::dot(Ro, Ro) - pow(this->radius,2);
        float d = pow(b, 2.0f) - (4.0f * a * c);
        if (d >= 0){
            // looking for closest positive intersection
            float t1 = (-b - sqrt(d)) / (2.0f * a);
            float t2 = (-b + sqrt(d)) / (2.0f * a);
            
            // intersection found
            if (t1 > tmin && t1 < h.getT()){
                // update hit
                // t and material modified
                Vector3f normal = (Ro + t1 * Rd);
                h.set(t1, this->material, normal);
                return true;
            }
            else if (t2 > tmin && t2 < h.getT()){
                // update hit
                // t and material modified
                Vector3f normal = (Ro + t2 * Rd);
                h.set(t2, this->material, normal);
                return true;
            }
        }
        return false;
	}
    

protected:
    Vector3f center;
    float radius;
};


#endif

