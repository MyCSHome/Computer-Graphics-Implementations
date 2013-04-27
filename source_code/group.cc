#include "raycast.h"

// Constructor for a group of n objects.
Group::Group(int n) {

  this->_object = new Sphere*[n];
  this->_count  = n;
}

// Destructor.
Group::~Group() {

  if (this->_object == NULL) {
    return;
  }

  for (int i = 0; i < this->_count; i++) {
    delete this->_object[i];
  }

  delete[] this->_object;
}


// Insert an object into the array.
void Group::addObject(int index, Sphere *obj) 
{
  _object[index] = obj ;
}


bool Group::intersect(const Ray &r, Hit &h)
{
  bool x;
  for (int i = 0 ; i < _count ; i++)
    x =  _object[i]->intersect(r,h);
  return x;
}
