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
		this->center = center;
		this->w = direction.normalized();
		this->u = Vector3f::cross(w, up).normalized();
		this->v = Vector3f::cross(u, w).normalized();
		//calculate u and v
		this->angle= angle; //use the angle to create the image view?
	}

	virtual Ray generateRay( const Vector2f& point){ //generate a ray from a point on my image plane right
		float alpha = tan(this->angle / 2.0f);
        
        Vector3f rayDirection;
        rayDirection = point.x() * alpha * this->u + point.y() * alpha * this->v +  this->w;
        rayDirection.normalize();
		return Ray(this->center, rayDirection);
	}

	virtual float getTMin() const { 
		return 0.0f;
	}

private:
	Vector3f center;
	Vector3f w;
	Vector3f v;
	Vector3f u;
	float angle;
};

#endif //CAMERA_H
