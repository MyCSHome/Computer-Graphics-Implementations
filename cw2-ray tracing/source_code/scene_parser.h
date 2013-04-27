#ifndef _SceneParser_H_
#define _SceneParser_H_

#include "raycast.h"

class Group;
class Camera;
class Sphere;

#define MAX_PARSER_TOKEN_LENGTH 100

// ====================================================================
// ====================================================================

class SceneParser {

public:

  // CONSTRUCTORS, DESTRUCTOR & INITIALIZE
  SceneParser();
  SceneParser(const char *filename);
  ~SceneParser();

  // ACCESSORS
  Group* getGroup() { return group; }
  OrthographicCamera* getCamera() { return camera; }
  Vec3f getBackgroundColor() { return background_color; }
Vec3f getCurrentColor() { return current_object_color; }
private:

  // HELPER FUNCTIONS
  void initialize();
  int getToken(char token[MAX_PARSER_TOKEN_LENGTH]);
  void parseFile();
  Group* parseGroup();
  OrthographicCamera* parseOrthographicCamera();
  void parseBackground();
  void parseMaterial();
  Sphere* parseSphere();

  // REPRESENTATION
  Group *group;
  OrthographicCamera *camera;
  Vec3f background_color;

  Vec3f current_object_color;
  FILE *file;

};

// ====================================================================
// ====================================================================

#endif
