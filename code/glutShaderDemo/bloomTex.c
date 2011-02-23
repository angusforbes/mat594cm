#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>
#include <soil.h>
#include <stdio.h>
#include "shader.h"
#include "main.h"

GLuint tex1;
float transZ = -5.0;
float threshold = -1.0;
int kernelSizeA = 3;
int kernelSizeB = 3;
GLuint shaderProgram;


float lx, ly, lz, lw, sx, sy, xw, yh, xinc, yinc;

void display(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glUseProgram(shaderProgram);

  setLighting();
  setMaterial();

  glTranslatef(0.0, 0.0, transZ);

  bloomTexture();

  glutSwapBuffers();
}

void bloomTexture(void) {
  glUseProgram(shaderProgram);


  int tex1ID = glGetUniformLocation(shaderProgram, "tex1");
  int thresholdID = glGetUniformLocation(shaderProgram, "threshold");
  int kA_ID = glGetUniformLocation(shaderProgram, "kA");
  int kB_ID = glGetUniformLocation(shaderProgram, "kB");

  glActiveTexture(GL_TEXTURE0);
  glBindTexture( GL_TEXTURE_2D, tex1 );  
  glUniform1i(tex1ID, 0);
  glUniform1f(thresholdID, threshold);
  glUniform1i(kA_ID, kernelSizeA);
  glUniform1i(kB_ID, kernelSizeB);

 glBegin(GL_QUADS);
  glTexCoord2f(0,0); glVertex2f(-1,-1);
  glTexCoord2f(1,0); glVertex2f(1,-1);
  glTexCoord2f(1,1); glVertex2f(1,1);
  glTexCoord2f(0,1); glVertex2f(-1,1);
 glEnd();

  //Un-bind GLSL Program to return to fixed functionality
  glUseProgram(0);
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

GLuint loadTexture(char *filename) {
  GLuint tex = SOIL_load_OGL_texture
    (
     filename,
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y //| SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
     //0
    );

  /* check for an error during the load process */
  if( 0 == tex ) {
    printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
  }

  return tex;
}


float randfloat0_1() { 
    return rand()/((float)(RAND_MAX)+1); 
} 

float randfloat(float min, float max) { 
    if (min>max) { 
        return randfloat0_1()*(min-max)+max;     
    } 
    else { 
        return randfloat0_1()*(max-min)+min; 
    }     
} 

void init(void) {
  glBlendFunc(GL_SRC_ALPHA,GL_ONE);
  glEnable(GL_BLEND);

  glClearColor(0.0,0.0,0.0,0.0);
  //tex1 = loadTexture("earth.jpg");
  tex1 = loadTexture("hubble.jpg");
  //blurTex = loadTexture("gauss.jpg");

  //light position variables
  lx = 3.0;
  ly = 1.0;
  lz = 2.0;
  lw = 0.0;

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
      kernelSizeA -= 1;
      break;
    case GLUT_KEY_RIGHT:
      kernelSizeA += 1;
      break;
    case GLUT_KEY_UP:
      kernelSizeB += 1;
      //transZ += 0.1;
      break;
    case GLUT_KEY_DOWN:
      kernelSizeB -= 1;
      //transZ -= 0.1;
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

  printf("kA = %d, kB = %d\n", kernelSizeA, kernelSizeB);

}

void timer(int value) {
  //update things...


  /* threshold += .01; */
  /* if (threshold > 1.0) { threshold = 0.0; } */
  //printf("threshold = %f\n", threshold);

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


  shaderProgram = initializeShaders("bloomTex.vert", "bloomTex.frag");

  printf("shaderProgram = %d\n", shaderProgram);
  //begin OpenGL rendering
  glutMainLoop();
}


