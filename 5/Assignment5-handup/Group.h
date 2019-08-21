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

  Group(){

  }
	
  Group( int num_objects ){
	  this->num_objects = num_objects;
	  this->items = vector<Object3D*>(num_objects); //new thing. of said capacity
  }

  ~Group(){
   
  }

  virtual bool intersect(const Ray& r, Hit& h, float tmin) {
      bool returnval = false;
	  for (int i = 0; i < items.size(); i++) {
          if (items[i]->intersect(r, h, tmin)) {
              //cout << "returned true" << endl;
              returnval = true;
          }
	  }
        //cout << "returned False" << endl;
      return returnval;
   }
	
  void addObject( int index , Object3D* obj ){
	  this->items[index] = obj; //just stuff it there.
  }

  int getGroupSize(){ 
  
  }

 private:
	 int num_objects;
	 vector<Object3D*> items;

};

#endif
	
