
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
#include <iostream>

using namespace std;
///TODO: implement this class.
///Add more fields as necessary,
///but do not remove hasTex, normals or texCoords
///they are filled in by other components
class Triangle: public Object3D
{
public:
	Triangle();
        ///@param a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m):Object3D(m){
        this->a = a;
        this->b = b;
        this->c = c;
        hasTex = false;
	}

	virtual bool intersect( const Ray& ray,  Hit& hit , float tmin){
        Vector3f Ro = ray.getOrigin();
        Vector3f Rd = ray.getDirection();
        
        Matrix3f A1(a.x()-Ro.x(), a.x()-c.x(), Rd.x(),
                    a.y()-Ro.y(), a.y()-c.y(), Rd.y(),
                    a.z()-Ro.z(), a.z()-c.z(), Rd.z()
                    );
        
        Matrix3f A2(a.x()-b.x(), a.x()-Ro.x(), Rd.x(),
                    a.y()-b.y(), a.y()-Ro.y(), Rd.y(),
                    a.z()-b.z(), a.z()-Ro.z(), Rd.z()
                    );
        
        Matrix3f A3(a.x()-b.x(), a.x()-c.x(), a.x()-Ro.x(),
                    a.y()-b.y(), a.y()-c.y(), a.y()-Ro.y(),
                    a.z()-b.z(), a.z()-c.z(), a.z()-Ro.z()
                    );
        
        Matrix3f A(a.x()-b.x(), a.x()-c.x(), Rd.x(),
                   a.y()-b.y(), a.y()-c.y(), Rd.y(),
                   a.z()-b.z(), a.z()-c.z(), Rd.z()
                   );
        
        float beta = A1.determinant()/ A.determinant();
        float gamma = A2.determinant()/ A.determinant();
        float alpha = 1.0f - beta - gamma;
        float t = A3.determinant() / A.determinant();
        if ((beta+gamma) > 1 || beta < 0 || gamma < 0){
            return false;
        }
        
        if (t > tmin && t < hit.getT()){
            Vector3f normal = alpha * normals[0] + beta * normals[1] + gamma * normals[2];
            hit.set(t,this->material, normal);
            hit.setTexCoord(alpha * texCoords[0] + beta * texCoords[1] + gamma * texCoords[2]);
            return true;
        }
        
        return false;
	}
	bool hasTex;
	Vector3f normals[3];
	Vector2f texCoords[3];
    
protected:
    Vector3f a;
    Vector3f b;
    Vector3f c;
};

#endif //TRIANGLE_H
