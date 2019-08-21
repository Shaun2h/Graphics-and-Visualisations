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
        this->normal = normal;
        this->d = d; // note that D is actually negative cross product of origin to normal. Since ray origin is always 0....
        this->material = m;
	}
	~Plane(){}
	virtual bool intersect( const Ray& r , Hit& h , float tmin){
        //cout << "plane run" << endl;
        float one = Vector3f::dot(this->normal, r.getOrigin());
        float two = Vector3f::dot(this->normal,r.getDirection());
        //cout << one << endl;
        //cout << two << endl;
        if (two == 0) {
            //cout << "ray parallel" << endl;
            return false;
        }
        float tval = -1.0f *(one- this->d) / two;
        //cout << tval << endl;
        if (tval > tmin && tval<h.getT()) { //update
            //cout << "hits plane" << endl;
            h.set(tval, this->material, this->normal.normalized());

            return true;    
        }
        else {
            return false;
        }
	}

protected:
    Vector3f normal;
    float d;
    Material* material;
};
#endif //PLANE_H
		

