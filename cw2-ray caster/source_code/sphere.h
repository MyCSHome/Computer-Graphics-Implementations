// Class for Sphere subclass of Object3D

// Add any code you need here and in the corresponding .cc file.

#ifndef SPHERE_H_
#define SPHERE_H_

#include "raycast.h"

class Sphere : public Object3D{


public:

  // Default constructor.
  Sphere();

  // Constructor with parameters specified.
  Sphere(Vec3f centre, float radius, Vec3f color);

  // A concrete implementation of pure virtual function in parent
  // class.
  virtual bool intersect(const Ray &r, Hit &h);
  Vec3f getcolor()
  {
    return color ;
    /*
    Vec3f colour;
    
    colour[0]= color.x();
    colour[1]=color.y();
    colour[2]=color.z();
    return colour ;
    */
  }
private:

  Vec3f centre ; 
  float radius ;
  Vec3f color ;
};

#endif /* SPHERE_H_ */
