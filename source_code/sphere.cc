#include "raycast.h"


// Add any code you need here and in the corresponding header
// file.


Sphere::Sphere()
{ 
}


Sphere::Sphere(Vec3f sphere_centre, float sphere_radius, Vec3f sphere_color)
{
  centre = sphere_centre ;
  radius = sphere_radius ;
  color = sphere_color ;

}


bool Sphere::intersect(const Ray &r, Hit &h)
{

  Vec3f ray_origin = r.getOrigin();
  
  Vec3f ray_direction = r.getDirection();

  Vec3f delta_p(ray_origin,centre);

  float dot_dp = delta_p.Dot3(ray_direction);

  float dot_p = delta_p.Dot3(delta_p);

  float r_square = radius*radius ;

  float i = pow(dot_dp,2) - dot_p + r_square;

  if(i < 0)
    return false ;

  else
    {
        
      float length_1 = -dot_dp + sqrt(i);
      float length_2 = -dot_dp - sqrt(i); 
      float min;
      
      if ((length_1 < 0) && (length_2 < 0))
	return false ;   
      else if (length_1*length_2<0)
	min = length_1 > 0 ? length_1 :length_2; 
      else 
        min = length_1<length_2 ? length_1 : length_2;
      
      float t = h.getT();
     
      if (t > min)
	h.set(min,color) ;
      
      return true ;
    }

}
