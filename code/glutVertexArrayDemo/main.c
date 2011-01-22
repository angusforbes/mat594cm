#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>
#include <soil.h>
#include <stdio.h>

void keyboard(unsigned char key, int x, int y);
void timer(int value);
void immediateSquare(void);
void vertexArraySquare(void);
void loadTextures(void);

GLuint tex;
float rotY = 0.0;
float transZ = -5.0;

float colors[] = {
  1.0, 0.0, 0.0, 1.0, 
  0.0, 1.0, 0.0, 1.0, 
  0.0, 0.0, 1.0, 1.0, 
  1.0, 1.0, 0.0, 1.0 };
float vertices[] = {
  -1.0, -1.0, 0.0, 
  1.0, -1.0, 0.0,
  1.0, 1.0, 0.0,
  -1.0, 1.0, 0.0 };
float texcoords[] = {
  0.0, 0.0, 
  2.0, 0.0,
  2.0, 2.0,
  0.0, 2.0 };

void display(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, transZ);
  glRotatef(rotY, 0.0, 1.0, 0.0);

  vertexArraySquare();

  glutSwapBuffers();
}


void vertexArraySquare() {

  glEnable( GL_TEXTURE_2D );
  //defaults are GL_MODULATE, and GL_CLAMP in both directions
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glBindTexture( GL_TEXTURE_2D, tex );    


  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);

  glColorPointer(4, GL_FLOAT, 0, colors);
  glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
  glVertexPointer(3, GL_FLOAT, 0, vertices);

  glDrawArrays(GL_QUADS, 0, 4);

  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

}

void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (float)width / (float) height, .01, 100.0);
}

void idle(void)
{
  glutPostRedisplay();
}


void loadTextures() {
  
  tex = SOIL_load_OGL_texture
    (
     "img_test.jpg",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
    );

  /* check for an error during the load process */
  if( 0 == tex )
  {
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

int main(int argc, char** argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(640, 480);

  glutCreateWindow("GLUT Program");

  init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(keySpecial);
  glutTimerFunc(1000/60,timer,0);

  glutMainLoop();
  return EXIT_SUCCESS;
}


