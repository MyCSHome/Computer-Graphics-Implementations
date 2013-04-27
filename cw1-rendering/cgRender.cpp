#include <cgRender.h>
#include <cstring>

float LightPosition[4] = {1.0, 0.0, 0.0, 1.0};
float LightAmbient[4] = {1.0, 1.0, 1.0, 1.0};
float LightDiffuse[4] = {1.0, 1.0, 1.0, 1.0};
float LightSpecular[4] = {0.0, 0.0, 0.0, 0.0};
float MaterialSpecular[4] = {1.0, 1.0, 1.0, 1.0};
float MaterialShininess[4] = {1.0, 0.0, 0.0, 1.0};


float fovy = 5;
float aspect = 1.0;
float near = 2;
float far = 4.0;

float eye[3] = {4, 0.1, 0.5};
float center[3] = {1.0, -0.1, 0.0};
float up[3] = {1.0, 1.0, 0.0};


void calculateNormals(float vertexInPolygon[][3],float normal[]) 
{
  //this function will return normalVector as an array of (floatx,floaty,floatz)
  float*  vertex1 = vertexInPolygon[0] ;
  float* vertex2 = vertexInPolygon[1] ;
  float* vertex3 = vertexInPolygon[2] ;

  float newVector1[3] = {vertex1[0]-vertex3[0],vertex1[1]-vertex3[1],vertex1[2]-vertex3[2] } ;
  float newVector2[3] = {vertex2[0]-vertex3[0],vertex2[1]-vertex3[1],vertex2[2]-vertex3[2] } ;

 
  normal[0] = newVector1[1]*newVector2[2] - newVector1[2]*newVector2[1];
  normal[1] = -newVector1[0]*newVector2[2] +newVector1[2]*newVector2[0];
  normal[2] = newVector1[0]*newVector2[1] - newVector1[1]*newVector2[0];
}

GLuint loadTexture(const char filename[],int width, int height)
{
  GLuint texture ;
  //open the filename 
  //ppm method !
  unsigned char* data ;
  FILE* file ;
  file = fopen(filename,"rb");
  if (file != NULL)
    return 0 ;
  data = (unsigned char*)malloc(width*height*3);
  fread(data,width*height*3,1,file);
  fclose(file) ;

  //to generate a texture
  glGenTextures(1,&texture); 

  glBindTexture(GL_TEXTURE_2D,texture);

  //set texture environment parameter
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);

  free(data);

  return texture ;
}


void init() 
{
  glClearColor (0.0, 0.0, 0.0, 0.0);
  cout << "init" << endl;

  
  glShadeModel (GL_SMOOTH);
  glEnable(GL_NORMALIZE);
  
 
  // Enable lighting
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmbient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDiffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
  
  // Set material parameters
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  MaterialSpecular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);
  
  // Enable Z-buffering
  glEnable(GL_DEPTH_TEST);
  
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  cout << "display" << endl;

  ifstream inOriginal ;		// need #include <fstream>
  inOriginal.open("data/face.vtk") ;
  
  char keyword1[30] ;
  int numberOfVertices ;

  inOriginal >> keyword1 ;
  while (strcmp(keyword1,"POLYDATA") != 0)	// need #include <cstring>
    inOriginal >> keyword1 ;
  inOriginal >> keyword1 ; // now contains 'POINT' word
  inOriginal >> numberOfVertices ;
  inOriginal >> keyword1 ; // now contains 'float' word
  
  float pointArray[numberOfVertices][3] ;
  for (int i = 0 ; i < numberOfVertices ; i++)
    {
      for (int j = 0 ; j < 3 ; j++)
	{
	  inOriginal >> pointArray[i][j] ;
	}
    }
  //NOW pointArray contains all point within 2D array [26450][3] 


  char keyword2[30] ;
  int numberOfPolygons ;
  int numberOfCells ; 
 
  inOriginal >> keyword2 ;
  cout << keyword2 ;  
  
  //keyword2 is "POLYGONS"
  inOriginal >> numberOfPolygons  ;  
  inOriginal >> numberOfCells   ;  
  
  int tempNum ;
  int polygonsArray[numberOfPolygons][3] ;
  for (int i = 0 ; i < numberOfPolygons ; i++)
    {
      inOriginal >> tempNum ;
      for (int j = 0 ; j < 3 ; j++)
	{
	  inOriginal >> polygonsArray[i][j] ;
	}
    }
  //NOW polygonsArray contains all polygons within 2D array [52260][3] 

  char keyword3[30] ;
  int pointWithTexture ;
  char tempWord[20] ;

  inOriginal >> keyword3 ; // keyword3 should be "POINT_DATA"
  inOriginal >> pointWithTexture ;

  for (int i = 0 ; i < 4 ; i++)
    inOriginal >> keyword3;

  float texturePointArray[pointWithTexture][2] ;
  for (int i = 0 ; i < pointWithTexture ; i++)
    {
      for (int j = 0 ; j < 2 ; j++)
	{
	  inOriginal >> texturePointArray[i][j] ;
	}
    }
  //NOW texturePointArray contains all texture-points within 2D array [26460][2] 


  inOriginal.close() ;

  //run loop for every polygon
  for (int k = 0 ; k < numberOfPolygons ; k++)
    {
      int vertexIndex[3] ;
      //This array keeps index in array size of 3 e.g. (2,198,33)
      vertexIndex[0] = polygonsArray[k][0] ;
      vertexIndex[1] = polygonsArray[k][1] ;
      vertexIndex[2] = polygonsArray[k][2] ;

      //search for actual vertex for each index
      
      float vertexInPolygon[3][3] ;
      float coordinate[3] ;
      int index ;

      //For texture
      GLuint texture = loadTexture("data/face.ppm",512,512);
      glEnable(GL_TEXTURE_2D);
      //glTexEnvf(...); not sure what it means
      glBindTexture(GL_TEXTURE_2D,texture);


      glBegin(GL_POLYGON);
      

      for (int a = 0 ; a < 3; a++) //based on vertexIndex array
	{
	  index = vertexIndex[a] ; 
	  //search from pointArray
	  
	  for (int m = 0 ; m < 3 ; m++)
	    coordinate[m] = pointArray[index][m];
	  // NOW coordinate[3] contains values of x y z of a point!!
	  //cout<<coordinate[0]<<endl;

	  glVertex3f(coordinate[0],coordinate[1],coordinate[2]) ;
	  
	   // Define texture coordinates of vertex
	  glTexCoord2f(texturePointArray[index][0],texturePointArray[index][1]);

	  //keep point in a polygon to calculate normal vector
	  vertexInPolygon[a][0] = coordinate[0] ;
	  vertexInPolygon[a][1] = coordinate[1] ;
	  vertexInPolygon[a][2] = coordinate[2] ;
 
	 
	  
	}

      // function for normal vector here (a plane has one normal vector (x,y,z))
      //NOW there is a polygon to find for its normals
      float normalVector[3] ;
      calculateNormals(vertexInPolygon,normalVector) ;

      // Define normal of vertex
      glNormal3f(normalVector[0],normalVector[1],normalVector[2]);
   
      
      
      glEnd();
    }
  

  glFlush();
  //For texture, to clear it to save the system resource
  glDisable(GL_TEXTURE_2D);





 

  /*
  for (all polygons)
    glBegin(GL_POLYGON);
    for (all vertices of polygon)

	// Define coordinates of vertex
      glVertex3f(...);
      
      // Define normal of vertex
      //glNormal3f(...);


      // Define texture coordinates of vertex
      glTexCoord2f(...);
      
      
    }
    glEnd();
  }
  */
  
  //  or, if double buffering is used,
  //  glutSwapBuffers();
  
}

void reshape (int w, int h)
{


  cout << "reshape" << endl;

  glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
  
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fovy, aspect, near, far);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
  
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 27: // ESC
    exit(0);
    break;
 case '1': // ESC
    eye[0]=eye[0]+0.001;
	break;
  case '2': // ESC
   eye[0]=eye[0]-0.001;
	break;
  case '3': // ESC
    eye[1]=eye[1]+0.001;
	break;
  case '4': // ESC
    eye[1]=eye[1]-0.001;
  case '5': // ESC
    eye[2]=eye[2]+0.001;
	break;
  case '6': // ESC
    eye[2]=eye[2]-0.001;
    break;
  case 'q': // ESC
    center[0]=center[0]+0.001;
	break;
  case 'w': // ESC
   center[0]=center[0]-0.001;
	break;
  case 'e': // ESC
    center[1]=center[1]+0.001;
	break;
  case 'r': // ESC
    center[1]=center[1]-0.001;
  case 't': // ESC
    center[2]=center[2]+0.001;
	break;
  case 'y': // ESC
    center[2]=center[2]-0.001;
    break;
  case 'a': // ESC
    up[0]=up[0]+0.1;
	break;
  case 's': // ESC
    up[0]=up[0]-0.1;
  case 'd': // ESC
    up[1]=up[1]+0.1;
	break;
  case 'f': // ESC
    up[1]=up[1]-0.1;
  case 'g': // ESC
    up[2]=up[2]+0.1;
	break;
  case 'h': // ESC
    up[2]=up[2]-0.1;
    break;
  case 'z': // ESC
    fovy=fovy+0.1;
	break;
  case 'x': // ESC
    fovy=fovy-0.1;
    break;
  case 'c': // ESC
    aspect=aspect+0.01;
	break;
  case 'v': // ESC
    aspect=aspect-0.01;
    break;
  case 'b': // ESC
    near=near+0.1;
	break;
  case 'n': // ESC
    near=near-0.1;
    break;
 case 'm': // ESC
    far=far+0.001;
	break;
  case ',': // ESC
    far=far-0.001;
    break;
}


 glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fovy, aspect, near, far);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
    glutPostRedisplay();

  }

int main(int argc, char** argv)
{
  // Initialize graphics window
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); 
  //  Or, can use double buffering
  //  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 

  glutInitWindowSize (256, 256); 
  glutInitWindowPosition (0, 0);
  glutCreateWindow (argv[0]);

  // Initialize OpenGL
  init();

  // Initialize callback functions
  glutDisplayFunc(display); 
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  // Start rendering 
  glutMainLoop();
}
