
// Class to represent a collection of objects in a scene.

#ifndef GROUP_H_
#define GROUP_H_

#include "raycast.h"


class Group : public Sphere{

 private:
  // An array of pointers to objects.
  Sphere **_object;

  // The number of objects in the group.
  int _count;

public:

  // Default constructor.
  Group(){this->_object = NULL; this->_count = 0;};

  // Constructor for a group of n objects.
  Group(int n);

  // Destructor.
  ~Group();

  // Insert an object into the array.
  void addObject(int index, Sphere *obj);

  bool intersect(const Ray &r, Hit &h);

};

#endif /* GROUP_H_ */
