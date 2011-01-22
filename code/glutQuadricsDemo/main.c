#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>
#include <soil.h>
#include <stdio.h>

void keyboard(unsigned char key, int x, int y);
void keySpecial(int key, int x, int y);
void idle(void);
void display(void);
void reshape(int width, int height);
void timer(int value);
void immediateSquare(void);
void loadTextures(void);

GLUquadricObj *qObj;
GLuint tex;
float rotY = 0.0;
float transZ = -5.0;

void display(void) {

  float r,x,y,a;
  int th,step;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, transZ);
  glRotatef(rotY, 0.0, 1.0, 0.0);

  displayQuadric();

  glutSwapBuffers();
}

void displayQuadric() {
  glEnable( GL_TEXTURE_2D );
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture( GL_TEXTURE_2D, tex );    

  //...sphere
  //glRotatef(270.0, 1.0, 0.0, 0.0);
  //gluSphere(qObj, 1.0, 64, 64);
  
  
  //..cylinder.
  glTranslatef(0.0, -1.0, 0.0);
  glRotatef(270.0, 1.0, 0.0, 0.0);
  gluCylinder(qObj, 1.0, .5, 2.0, 64, 64);
  
}

void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (float)width / (float) height, .01, 100.0);
}

void loadTextures() {
  tex = SOIL_load_OGL_texture (
      "imgs/img_test.jpg",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y 
      );

  // check for texture loading error
  if( 0 == tex ) {
    printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
  }
}

void initQuadrics() {
  qObj = gluNewQuadric(); //load up new quadric 
  //gluQuadricDrawStyle(qObj, GLU_FILL); //fill in the facets on the surface 
  gluQuadricNormals(qObj, GLU_SMOOTH); //make nice normals across the facets
  //if lighting is enabled then the normals are already present
  gluQuadricTexture(qObj, GL_TRUE); //automatically create texture coords for the object
  //if you a bind a texture, then the texture coords are already present
}

void init(void) {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0,0.0,0.0,0.0);
  initQuadrics();
  loadTextures();
}


void keyboard(unsigned char key, int x, int y) {
  printf("key pressed %c (%d) \n", key, key);

  switch(key) {
    case 'a':
      break;
    
    case 'b':
      break;

    case 'q': 
    case 27: //Escape key
      exit(0); 
  }
}

void keySpecial(int key, int x, int y) {  
  printf("special key pressed:< %c > \n", key);

  switch(key) {
    case GLUT_KEY_LEFT:
      break;
    case GLUT_KEY_RIGHT:
      break;
    case GLUT_KEY_UP:
      transZ += 0.1;
      break;
    case GLUT_KEY_DOWN:
      transZ -= 0.1;
      break;
    case GLUT_KEY_HOME:
      break;
    case GLUT_KEY_END:
      break;
    case GLUT_KEY_PAGE_UP:
      break;
    case GLUT_KEY_PAGE_DOWN:
      break;
  }
}


void timer(int value) {
  //update things...
  rotY += 1.0;

  //then wait a few milliseconds before calling yourself again
  glutTimerFunc(1000/60,timer,0);
}

void idle(void) {
  //tell OpenGL to redraw whenever its not too busy doing something else
  glutPostRedisplay();
}


int main(int argc, char** argv) {
  //set up GLUT context
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(640, 480);
  glutCreateWindow("basic GLUT Program");

  //set up GLUT callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(keySpecial);
  glutTimerFunc(1000/60,timer,0);

  //initialize state and load in resources, etc.
  init();

  //begin OpenGL rendering
  glutMainLoop();
}


