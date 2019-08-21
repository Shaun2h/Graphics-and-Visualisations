#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>

using  namespace std;

///TODO: 
///Implement Group
///Add data structure to store a list of Object* 
class Group:public Object3D
{
public:
    // store all objects in the entire scene
    Group(){
        num_objects = 0;
    }

    Group( int num_objects ){
        num_objects = num_objects;
    }
    
    // destructor: destroy the instance and frees up memory
    ~Group(){
        for (int i = 0; i < getGroupSize(); i++){
            delete object_list[i];
        }
    }

    virtual bool intersect( const Ray& r , Hit& h , float tmin ) {
        // loop through all instances calling their intersection methods
        bool returnval = false;
        for (int i = 0; i < getGroupSize(); i++){
            Object3D* obj = object_list[i];
            bool val = obj->intersect(r, h, tmin);
            if (val){
                returnval=true;
            }
        }
        return returnval;
    }

    void addObject( int index , Object3D* obj ){
        object_list.push_back(obj);
    }

    int getGroupSize(){
        return object_list.size();
    }

 private:
    vector<Object3D*> object_list;
    int num_objects;

};

#endif
	
