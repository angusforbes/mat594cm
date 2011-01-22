#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>
#include <soil.h>
#include <stdio.h>
#include "main.h"

GLuint tex;
float rotY = 0.0;
float transZ = -5.0;

float vs[] = {-1, -1, 0, 1, -1, 0, 1, 1, 0, -1, 1, 0};
float *ns; 

void display(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, transZ);
  glRotatef(rotY, 0.0, 1.0, 0.0);

  immediateSquare();

  glutSwapBuffers();
}


void normalizeNormal(float n[]) {
  float len = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
  n[0] /= len;
  n[1] /= len;
  n[2] /= len;
}

void computeNormal(float v1[], float v2[], float n[]) {
  n[0] = v1[1] * v2[2] - v2[1] * v1[2];
  n[1] = v1[2] * v2[0] - v2[2] * v1[0];
  n[2] = v1[0] * v2[1] - v2[0] * v1[1];
  normalizeNormal(n);
} 



void immediateSquare(void) {
  glEnable( GL_TEXTURE_2D );
 // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glBindTexture( GL_TEXTURE_2D, tex );    

  glBegin( GL_QUADS ); {
    glColor4f(1.0,0.0,0.0,1.0);
    glTexCoord2f(0, 0);
    glVertex3f(-.5, -.5, 0);
    
    glColor4f(0.0,1.0,0.0,1.0);
    glTexCoord2f(1, 0);
    glVertex3f(.5, -.5, 0);
    
    glColor4f(0.0,0.0,1.0,1.0);
    glTexCoord2f(1, 1);
    glVertex3f(.5, .5, 0);
    
    glColor4f(1.0,1.0,0.0,1.0);
    glTexCoord2f(0, 1);
    glVertex3f(-.5, .5, 0);
  }
  glEnd();

  glDisable( GL_TEXTURE_2D );
  
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

void init(void) {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0,0.0,0.0,0.0);
  loadTextures();

  //compute normals...
  int p0off = 0; 
  int p1off = 3; 
  int p3off = 10;

  float v1[] = {vs[p1off + 0] - vs[p0off + 0], vs[p1off + 1] - vs[p0off + 1], vs[p1off + 2] - vs[p0off + 2]};
  float v2[] = {vs[p3off + 0] - vs[p0off + 0], vs[p3off + 1] - vs[p0off + 1], vs[p3off + 2] - vs[p0off + 2]};

  float* n = (float*)malloc(3 * sizeof(float));
  computeNormal(v1, v2, n);
  
  printf("%f/%f/%f \n", n[0], n[1], n[2]);

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

void mouseMoved(int x, int y) {
  printf("mouse at %i/%i\n", x, y);
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
  //glutPassiveMotionFunc(mouseMoved);
  glutTimerFunc(1000/60,timer,0);

  //initialize state and load in resources, etc.
  init();
  //loadTextures();
  //begin OpenGL rendering
  glutMainLoop();
}


