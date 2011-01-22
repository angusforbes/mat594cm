#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>
#include <soil.h>
#include <stdio.h>
#include "main.h"

GLuint tex;
float rotY = 0.0;
float transZ = -5.0;

void display(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, transZ);
  glRotatef(rotY, 0.0, 1.0, 0.0);

  immediateSquare();

  glutSwapBuffers();
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
  
  //begin OpenGL rendering
  glutMainLoop();
}


