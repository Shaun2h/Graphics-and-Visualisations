#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

#include <iostream>
using namespace std;
///TODO:
///Implement functions and add more fields as necessary
class Sphere: public Object3D
{
public:
	Sphere(){ 
		//unit ball at the center
		this->center = Vector3f(0.0, 0.0, 0.0);
		this->radius = 1.0;
	}

	Sphere( Vector3f center , float radius , Material* material ):Object3D(material){
		this->center = center;
		this->radius = radius;
		this->material = material;
	}
	

	~Sphere(){}

	virtual bool intersect( const Ray& r , Hit& h , float tmin){
		Vector3f rayorigin = r.getOrigin()- this->center;
		float a = Vector3f::dot(r.getDirection(),r.getDirection()); //just following the equation
		float b = 2 * Vector3f::dot(rayorigin, (r.getDirection()));
		float c = Vector3f::dot(rayorigin, rayorigin)-pow(this->radius, 2.0f);
		float determinant = pow(b, 2) - 4 * a * c;
		float selectedT;
        if (determinant > 0) {
            float t1 = (-b + sqrt(determinant)) / 2 * a;
            float t2 = (-b - sqrt(determinant)) / 2 * a;
            if (t2 <= t1 && t2>=tmin) {
                selectedT = t2;
            }
            else if(t1>=tmin && t1<=t2){
                selectedT = t1;
            }
            else {
                return false;
            }
            Vector3f normal = (r.pointAtParameter(selectedT) - this->center).normalized();
            //update Hit

            /*
            //Vector3f pointAtParameter(float t) const //really just means intersection point from ray.
            //We need to move this intersection by how much the circle is moved too though.
            {
                return origin + direction * t;
            }
            */

            if (selectedT < h.getT()) {
                //cout<< "HIT HIT HIT"<<endl;
                h.set(selectedT, this->material, normal);
                return true;
            }
            //cout << "FAILED TMIN CHECK" << endl;
            return false;
		}
		else {
            //cout << "failed to even hit" << endl;
            return false;
			
		}

		/*
		Vector3f raydirection = r.getDirection();
		Vector3f rayorigin = r.getOrigin();
		Vector3f translated_rayorigin = this->center - rayorigin + tmin*raydirection; // we now have the new ray origin, while the sphere is centered about 0 0 0.
		bool xray = raydirection.x != 0;
		bool yray = raydirection.y != 0;
		bool zray = raydirection.z != 0;
		bool beforex = translated_rayorigin.x < -this->radius;
		bool afterx = translated_rayorigin.x > this->radius;
		bool beforey = translated_rayorigin.y < -this->radius;
		bool aftery = translated_rayorigin.y > this->radius;
		bool beforez = translated_rayorigin.z < -this->radius;
		bool afterz = translated_rayorigin.z > this->radius;

		float kept_t=0;

		if (beforex && !afterx && xray) { // ray is not parallel, begins before the sphere
			if (raydirection.x > 0) {
				//intersection happens
				float t1 = (translated_rayorigin.x + this->radius)/raydirection.x; 
				kept_t = t1; // assign newest tvalue.
			}
			//else, no intersection happens.
		}
		else {
			if (!beforex && afterx && xray) { //ray is not parallel, begins after sphere
				if (raydirection.x < 0) {//intersection happens
					float t1 = (translated_rayorigin.x - this->radius)/raydirection.x;
					kept_t = -t1; // it's the negative of this. ray direction goes <, translated_rayorigin.x - radius
				}
				//else no intersection here.
			}
			else {
				if (!beforex && !afterx && xray) { //ray is not parallel, begins within the sphere
					//intersection happens no matter what.
					float t1 = (this->radius - translated_rayorigin.x) / raydirection.x;
					float t2 = (-this->radius - translated_rayorigin.x) / raydirection.x;
					if (t1 > 0) { //it intersects on whatever wall, in specified t value.
						kept_t = t1;
					}
					else {
						kept_t = t2;
					} 
				}
			}
		}
		// we might have obtained the first t value, where moving along this axis in direction after tmin. anyway do this for x y and z


		//********************************************* Y RAY
		
		if (beforey && !aftery && yray) { // ray is not parallel, begins before the sphere
			if (raydirection.y > 0) {
				//intersection happens
				float t1 = (translated_rayorigin.y + this->radius) / raydirection.y;
				if (kept_t>t1){
					kept_t = t1; // assign newest tvalue.
				}
			}
			//else, no intersection happens.
		}
		else {
			if (!beforey && aftery && yray) { //ray is not parallel, begins after sphere
				if (raydirection.y < 0) {//intersection happens
					float t1 = (translated_rayorigin.y - this->radius) / raydirection.y;
					if (kept_t >-t1){
						kept_t = -t1; // it's the negative of this. ray direction goes <, translated_rayorigin.x - radius
					}
				}
				//else no intersection here.
			}
			else {
				if (!beforey && !aftery && yray) { //ray is not parallel, begins within the sphere
					//intersection happens no matter what.
					float t1 = (this->radius - translated_rayorigin.y) / raydirection.y;
					float t2 = (-this->radius - translated_rayorigin.y) / raydirection.y;
					if (t1 > 0 && kept_t>t1) { //it intersects on whatever wall, in specified t value.
						kept_t = t1;
					}
					else {
						if (kept_t>t2) {
							kept_t = t2; 
						}
					}
				}
			}
		}



		//********************************************* Z RAY

		if (beforez && !afterz && zray) { // ray is not parallel, begins before the sphere
			if (raydirection.z > 0) {
				//intersection happens
				float t1 = (translated_rayorigin.z + this->radius) / raydirection.z;
				if (kept_t > t1) {
					kept_t = t1; // assign newest tvalue.
				}
			}
			//else, no intersection happens.
		}
		else {
			if (!beforez && afterz && zray) { //ray is not parallel, begins after sphere
				if (raydirection.z < 0) {//intersection happens
					float t1 = (translated_rayorigin.z - this->radius) / raydirection.z;
					if (kept_t > -t1) {
						kept_t = -t1; // it's the negative of this. ray direction goes <, translated_rayorigin.x - radius
					}
				}
				//else no intersection here.
			}
			else {
				if (!beforez && !afterz && zray) { //ray is not parallel, begins within the sphere
					//intersection happens no matter what.
					float t1 = (this->radius - translated_rayorigin.z) / raydirection.z;
					float t2 = (-this->radius - translated_rayorigin.z) / raydirection.z;
					if (t1 > 0 && kept_t > t1) { //it intersects on whatever wall, in specified t value.
						kept_t = t1;
					}
					else {
						if (kept_t > t2) {
							kept_t = t2;
						}
					}
				}
			}
		}

		kept_t += tmin; //add tmin to new value.

		if (kept_t != 0 && h.getT>kept_t) {
			h.set(kept_t, this->material, r.pointAtParameter(kept_t).normalized()); //reset the h value to the new stuff.
			return true;
		}
		else {
			return false; 
		}
		*/
	}

protected:
	Vector3f center;
	float  radius;
	Material* material;
};


#endif
