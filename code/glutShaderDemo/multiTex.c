#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>
#include <soil.h>
#include <stdio.h>
#include "shader.h"
#include "main.h"

GLuint tex1, tex2, blurTex;
float transZ = -5.0;
GLuint shaderProgram;

float BLUR_OPACITY = .5;
float BLUR_MARGIN = .5;

float lx, ly, lz, lw, sx, sy, xw, yh, xinc, yinc;

int cols = 15;
int rows = 15;

struct tile
{
  float rate, xpos, ypos, opacity;
} tiles[15][15];

//struct tile tiles[15][15];

void display(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glUseProgram(shaderProgram);

  setLighting();
  setMaterial();

  glTranslatef(0.0, 0.0, transZ);

  multiTexture();

  glutSwapBuffers();
}


void drawMultiTexturedTile(
    float x, float y,
    float w, float h,
    float tL, float tR,
    float tB, float tT
    )
{
  float bx = .15;
  float by = .15;
  glMultiTexCoord2f(GL_TEXTURE1, tL, tB);
  glMultiTexCoord2f(GL_TEXTURE2, tL, tB);
  glMultiTexCoord2f(GL_TEXTURE0, bx,by);
  glVertex2f(x, y);

  glMultiTexCoord2f(GL_TEXTURE1, tR, tB);
  glMultiTexCoord2f(GL_TEXTURE2, tR, tB);
  glMultiTexCoord2f(GL_TEXTURE0, 1.0-bx,by);
  glVertex2f(x + w, y);

  glMultiTexCoord2f(GL_TEXTURE1, tR, tT);
  glMultiTexCoord2f(GL_TEXTURE2, tR, tT);
  glMultiTexCoord2f(GL_TEXTURE0, 1.0-bx, 1.0-by);
  glVertex2f(x + w, y + h);

  glMultiTexCoord2f(GL_TEXTURE1, tL, tT);
  glMultiTexCoord2f(GL_TEXTURE2, tL, tT);
  glMultiTexCoord2f(GL_TEXTURE0, bx,1.0-by);
  glVertex2f(x, y + h);
}

void multiTexture(void) {

 
  glUseProgram(shaderProgram);

  int opacID = glGetAttribLocation(shaderProgram, "opac");
  //int blurOpacID = glGetAttribLocation(shaderProgram, "blurOpac");
  int blurTexID = glGetUniformLocation(shaderProgram, "blurTex"); 
  int tex1ID = glGetUniformLocation(shaderProgram, "tex1");
  int tex2ID = glGetUniformLocation(shaderProgram, "tex2");

   glEnable(GL_TEXTURE_2D);  

  glActiveTexture(GL_TEXTURE0);
  glBindTexture( GL_TEXTURE_2D, blurTex );  

  glActiveTexture(GL_TEXTURE1);
  glBindTexture( GL_TEXTURE_2D, tex1 );  

  glActiveTexture(GL_TEXTURE2);
  glBindTexture( GL_TEXTURE_2D, tex2 );  

  glUniform1i(blurTexID, 0);
  glUniform1i(tex1ID, 1);
  glUniform1i(tex2ID, 2);

  float twInc = (1.0) / cols;
  float thInc = (1.0) / rows;
  float wInc = xw / cols;
  float hInc = yh / rows;

   glBegin(GL_QUADS);

  int i, j;
  for (i = 0; i < cols; i++)
  {
    for (j = 0; j < rows; j++)
    {
      glVertexAttrib1f(opacID, tiles[i][j].opacity);

      float margin = .0;
      tiles[i][j].opacity += tiles[i][j].rate;

      if (tiles[i][j].opacity > 1.0 || tiles[i][j].opacity < 0.0)
      {
	tiles[i][j].rate *= -1.0;
      }

      drawMultiTexturedTile(
	  tiles[i][j].xpos, 
	  tiles[i][j].ypos,
	  wInc, 
	  hInc,
	  twInc * i, 
	  twInc * (i + 1), 
	  0.0 + (thInc * j), 
	  0.0 + (thInc * (j + 1))
	  );

    }
  }

  glEnd();

  glActiveTexture(GL_TEXTURE0);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE2);
  glDisable(GL_TEXTURE_2D);

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

void initializeTiles(void) {
  
  //tile position variables
  sx = -2.0;
  sy = -2.0;
  xw = 4.0;
  yh = 4.0;
  xinc = xw / cols;
  yinc = yh / rows;

  //set up each tile
  int i, j;
  for (i = 0; i < cols; i++) {
    for (j = 0; j < rows; j++) {
      tiles[i][j].xpos = sx + (xinc * i);     
      tiles[i][j].ypos = sy + (yinc * j); 
      tiles[i][j].rate = randfloat(.0001, .0003);
      tiles[i][j].opacity = .5;
    }
  }
}

void init(void) {
  glBlendFunc(GL_SRC_ALPHA,GL_ONE);
  glEnable(GL_BLEND);

  glClearColor(0.0,0.0,0.0,0.0);
  tex1 = loadTexture("earth.jpg");
  tex2 = loadTexture("hubble.jpg");
  blurTex = loadTexture("gauss.jpg");

  //light position variables
  lx = 3.0;
  ly = 1.0;
  lz = 2.0;
  lw = 0.0;

   
  initializeTiles();

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


  shaderProgram = initializeShaders("multiText.vert", "multiText.frag");

  printf("shaderProgram = %d\n", shaderProgram);
  //begin OpenGL rendering
  glutMainLoop();
}


