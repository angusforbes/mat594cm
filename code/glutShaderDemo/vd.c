#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>
#include <soil.h>
#include <stdio.h>
#include "shader.h"
#include "main.h"

GLuint tex;
float rotY = 0.0;
float transZ = -5.0;
GLuint shaderProgram;
//light position variables
float lx = 3.0;
float ly = 1.0;
float lz = 2.0;
float lw = 0.0;

float waveNum = 0.0;

float sx = -3.0;
float ex = 3.0;
float numsquares = 100;


void display(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glUseProgram(shaderProgram);

  setLighting();
  setMaterial();
  
  glTranslatef(0.0, 0.0, transZ);
  //glRotatef(rotY, 0.0, 1.0, 0.0);

  float xinc = (ex - sx) / numsquares;

  int waveID = glGetAttribLocation(shaderProgram, "wave");
   
  printf("waveID = %d \n", waveID);
 glVertexAttrib1f(waveID, waveNum);

  glutSolidTeapot(2.0);
  /*
  glBegin(GL_QUADS);
  
  int i;
  for (i = 0; i < numsquares - 1; i++) {
    glVertexAttrib1f(waveID, waveNum);
    printf("waveNum = %f\n", waveNum);
    float x1 = sx + (xinc * i);    
    float x2 = sx + (xinc * (i+1));    

    glVertex3f(x1, -1.0, 0.0);
    glVertex3f(x2, -1.0, 0.0);
    glVertex3f(x2, 1.0, 0.0);
    glVertex3f(x1, 1.0, 0.0);


  }

  glEnd();
  */
  glUseProgram(0);


  waveNum += .01;



  glutSwapBuffers();
}

void setLighting(void) {
  float DiffuseLight[] = {1.0, 0.0, 0.0}; 
  float AmbientLight[] = {0.2, 0.2, 0.2};
  float SpecularLight[] = {1.0, 1.0, 1.0};
    
  glLightfv (GL_LIGHT0, GL_SPECULAR, SpecularLight); 
  glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight); 
  glLightfv (GL_LIGHT0, GL_AMBIENT, AmbientLight); 
    
  float LightPosition[] = {
    lx, ly, lz, lw  }; 

  glLightfv (GL_LIGHT0, GL_POSITION, LightPosition); 
}

void setMaterial(void) {
   
    float mShininess[] = {5};
    float DiffuseMaterial[] = {1.0, 0.0, 0.0}; 
    float AmbientMaterial[] = {0.0, 0.0, 0.0};
    float SpecularMaterial[] = {1.0, 1.0, 1.0}; 
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseMaterial);
    glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientMaterial);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularMaterial);
    glMaterialfv(GL_FRONT, GL_SHININESS, mShininess);


}

void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (float)width / (float) height, .01, 100.0);
}


void init(void) {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0,0.0,0.0,0.0);
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


  shaderProgram = initializeShaders("vd.vert", "vd.frag");

  printf("shaderProgram = %d\n", shaderProgram);
  //begin OpenGL rendering
  glutMainLoop();
}


