#include "raycast.h"

OrthographicCamera::OrthographicCamera(Vec3f camera_centre, Vec3f camera_direction, Vec3f camera_up, float camera_size)
{
  centre = camera_centre;
  direction = camera_direction ;
  direction.Normalize();
  size = camera_size;
  
  if(direction.Dot3(camera_up) == 0)
    {
      up = camera_up ;
      up.Normalize();
    }
    
  else 
    {  
      float parallel = direction.Dot3(camera_up) ;
      Vec3f parallel_vector = direction * parallel ;
      up = camera_up - parallel_vector ;
      up.Normalize() ;
    }

  
}


Ray OrthographicCamera::generateRay(Vec2f point)
{
  float mapping1,mapping2;
  
  mapping1 = -1 + 2*point.x();
  mapping2 = -1 + 2*point.y();

  Vec3f h = cal_horizontal();
    
  Vec3f origin = centre + up*mapping2 *(size/2)  + h*mapping1 * (size/2);

  Ray generated_ray(direction,origin);

  return generated_ray ;
}



Vec3f OrthographicCamera::cal_horizontal()
{
  Vec3f horizontal;
  Vec3f::Cross3(horizontal,direction,up);
  return horizontal;
}

