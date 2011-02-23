#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>
#include <soil.h>
#include <stdio.h>
#include "shader.h"
#include "main.h"

GLuint tex_color, tex_normal;
float transZ = -5.0;
GLuint shaderProgram;
float lx, ly, lz, lw;
float rotY = 0.0;

void display(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glUseProgram(shaderProgram);

  setLighting();
//  setMaterial();

  glTranslatef(0.0, 0.0, transZ);
  //glRotatef(rotY, 0.0, 1.0, 0.0);
  bumpMap();
//  multiTexture();

  glutSwapBuffers();

  rotY += 0.01;
}

void bumpMap(void) {

  glUseProgram(shaderProgram);


  glActiveTexture(GL_TEXTURE0);
  int texture_location = glGetUniformLocation(shaderProgram, "color_texture");
  glUniform1i(texture_location, 0);
  glBindTexture(GL_TEXTURE_2D, tex_color);

  glActiveTexture(GL_TEXTURE1);
  int normal_location = glGetUniformLocation(shaderProgram, "normal_texture");
  glUniform1i(normal_location, 1);
  glBindTexture(GL_TEXTURE_2D, tex_normal);

  //glutSolidTeapot(1.0);
  
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-1.0f, -1.0f, 0.0f);

  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(1.0f, -1.0f, 0.0f);

  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(1.0f, 1.0f, 0.0f);

  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-1.0f, 1.0f, 0.0f);
  glEnd();
  
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);

  //Un-bind GLSL Program to return to fixed functionality
  glUseProgram(0);
}

int dir = 1;

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

  lx += (0.001 * dir);

  printf("lx = %f\n", lx);
  if (lx > 5.0 || lx < -5.0) {
    dir *= -1;
  }

  
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


void init(void) {
  //glBlendFunc(GL_SRC_ALPHA,GL_ONE);
  //glEnable(GL_BLEND);
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glClearColor(0.0,0.0,0.0,0.0);
  //tex_color = loadTexture("brickwall_colormap.jpg");
  //tex_normal = loadTexture("brickwall_normalmap.png");
  tex_color = loadTexture("colour_map.png");
  //tex_normal = loadTexture("normal_map.png");
  //tex_color = loadTexture("retired-bricks.jpg");
  tex_normal = loadTexture("retired-bricks_normal.png");
  //tex_color = loadTexture("bricks_3.jpg");
  //tex_normal = loadTexture("bricks_3_normalmap.png");
  //tex_normal = loadTexture("white_normal.png");

  //light position variables
  lx = -5.0;
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


  shaderProgram = initializeShaders("bump2.vert", "bump2.frag");

  printf("shaderProgram = %d\n", shaderProgram);
  //begin OpenGL rendering
  glutMainLoop();
}


