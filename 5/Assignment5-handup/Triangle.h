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
          hasTex = false;
          this->a = a;
          this->b = b;
          this->c = c;

          this->material = m;
	}

	virtual bool intersect( const Ray& ray,  Hit& hit , float tmin){
        //cout << "triangle run" << endl;
        float rayx = ray.getOrigin().x();
        float rayy = ray.getOrigin().y();
        float rayz = ray.getOrigin().z();
        float directx = ray.getDirection().x();
        float directy = ray.getDirection().y();
        float directz = ray.getDirection().z();
        
        Matrix3f A1 = Matrix3f(this->a.x() - rayx, this->a.x() - this->c.x(), directx,
                       this->a.y() - rayy, this->a.y() - this->c.y(), directy,
                       this->a.z() - rayz, this->a.z() - this->c.z(), directz
            );
        Matrix3f A2 = Matrix3f(this->a.x() - this->b.x(), this->a.x() - rayx, directx,
                       this->a.y() - this->b.y(), this->a.y() - rayy, directy,
                       this->a.z() - this->b.z(), this->a.z() - rayz, directz
            );
        Matrix3f A3 = Matrix3f(this->a.x() - this->b.x(), this->a.x() - this->c.x(), this->a.x() - rayx,
                       this->a.y() - this->b.y(), this->a.y() - this->c.y(), this->a.y() - rayy,
                       this->a.z() - this->b.z(), this->a.z() - this->c.z(), this->a.z() - rayz
            );
        Matrix3f A = Matrix3f(this->a.x() - this->b.x(), this->a.x() - this->c.x(), directx,
                      this->a.y() - this->b.y(), this->a.y() - this->c.y(), directy,
                      this->a.z() - this->b.z(), this->a.z() - this->c.z(), directz
            );
        if (A.determinant()==0){
            return false;
        }
        //cout << "determinant is not 0" << endl;
        float beta = A1.determinant() / A.determinant();
        float gamma = A2.determinant() / A.determinant();
        float t = A3.determinant() / A.determinant();
        float alpha = 1.0f - beta - gamma;
        //cout << rayx << endl;
        //cout << rayy << endl;
        //cout << rayz << endl;
        //cout << directx << endl;
        //cout << directy << endl;
        //cout << directz << endl;
        //A.print();
        //A1.print();
        //A2.print();
        //A3.print();
        //this->a.print();
        //this->b.print();
        //this->c.print();
        //cout << A1.determinant() << endl;
        //cout << A2.determinant() << endl;
        //cout << A3.determinant() << endl;
        //cout << A.determinant() << endl;
        /*if (A1.determinant() != A1.determinant() || A2.determinant() != A2.determinant() || A3.determinant() != A3.determinant() || A1.determinant != A1.determinant()) {
            cout << "NAN detected" << endl;
            return false;
        }*/
        if (t >= tmin && t < hit.getT() && beta + gamma <= 1 && beta>=0 && gamma>=0) {//we can use this
            Vector3f newnormal = this->normals[0] * alpha + beta * this->normals[1] + gamma * this->normals[2];
            hit.set(t, this->material, newnormal.normalized());
            hit.setTexCoord(alpha * texCoords[0] + beta * texCoords[1] + gamma * texCoords[2]);
            //cout << "triangle hit" << endl;
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
    Material* material;
};

#endif //TRIANGLE_H
