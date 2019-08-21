#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
using namespace std;
///TODO: Implement Plane representing an infinite plane
///choose your representation , add more fields and fill in the functions
class Plane: public Object3D
{
public:
	Plane(){}
	Plane( const Vector3f& normal , float d , Material* m):Object3D(m){
        this->normal = normal.normalized();
        this->d = d;
	}
	~Plane(){}
	virtual bool intersect( const Ray& r , Hit& h , float tmin){
        // d is offset from the origin
        // plane equation is P.n = d
        Vector3f Ro = r.getOrigin();
        Vector3f Rd = r.getDirection();
        float ndotRo = Vector3f::dot(this->normal,Ro);
        float ndotRd = Vector3f::dot(this->normal,Rd);
        if (ndotRd == 0){
            return false;
        }
        
        float t = (this->d - ndotRo) / ndotRd;
        if(t > tmin && t < h.getT()){
            h.set(t, this->material, this->normal);
            return true;
        }
        
        return false;
	}

protected:
    Vector3f normal;
    float d;
};
#endif //PLANE_H
		

