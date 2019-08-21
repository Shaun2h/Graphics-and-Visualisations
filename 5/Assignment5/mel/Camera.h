
#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera
{
public:
	//generate rays for each screen-space coordinate
	virtual Ray generateRay( const Vector2f& point ) = 0 ; 
	
	virtual float getTMin() const = 0 ; 
	virtual ~Camera(){}
protected:
	Vector3f center; 
	Vector3f direction;
	Vector3f up;
	Vector3f horizontal;

};

///TODO: Implement Perspective camera
///Fill in functions and add more fields if necessary
class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vector3f& center, const Vector3f& direction,const Vector3f& up , float angle){
        // w = direction, u = w x up, v = u x w
        this->w = direction.normalized();
        this->u = Vector3f::cross(w,up).normalized();
        this->v = Vector3f::cross(u,w).normalized();
        this->center = center;
        this->angle = angle;
	}

	virtual Ray generateRay( const Vector2f& point){
        float D = 1.0f/tan(this->angle/2.0);
        //float a = tan(this->angle/2.0);
        Vector3f Rd = ((point.x() * this->u) + (point.y() * this->v) + (D * this->w)).normalized();
        return Ray(this->center, Rd);
	}

	virtual float getTMin() const { 
		return 0.0f;
	}

private:
    Vector3f u;
    Vector3f v;
    Vector3f w;
    Vector3f center;
    float angle;

};

#endif //CAMERA_H

