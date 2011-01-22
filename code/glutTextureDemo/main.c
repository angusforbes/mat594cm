#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>
#include <soil.h>
#include <stdio.h>
#define PI 3.14159265

void keyboard(unsigned char key, int x, int y);
void timer(int value);
void immediateSquare(void);
void vertexArraySquare(void);
void loadTextures(void);


GLUquadricObj *qObj;
GLuint tex;

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
  0.0, 0.0, 0.0, 
  1.0, 0.0, 0.0,
  1.0, 1.0, 0.0,
  0.0, 1.0, 0.0 };

void display(void) {

  float r,x,y,a;
  int th,step;

  a = 1.0;

  //	glPushMatrix();
  //	glRotatef(G_rot,0.0,0.0,1.0);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -5.0);

  
   //gluSphere(qObj, 2.0, 64, 64);
  //glutSolidTeapot(2.0);


  //immediateSquare();
  vertexArraySquare();

  /*
     glBegin( GL_LINE_STRIP );
     for (th = 0 ;th<360;th++){
     r=a*cos(G_k*th*(PI/180.0));
     x = r*cos(th*(PI/180.0));
     y = r*sin(th*(PI/180.0));
     glVertex2f(x,y);
     }
     glEnd();
     */

  //glBegin( GL_LINE_STRIP );
  //	for (th = 0,step =0 ;th<360;th++, step+=G_n){
  //		r=a*cos(G_k*step*(PI/180.0));
  //		x = r*cos(step*(PI/180.0));
  //		y = r*sin(step*(PI/180.0));
  //		glVertex2f(x,y);
  //	}
  //glEnd();



  //   glPopMatrix();


  glutSwapBuffers();
}


void vertexArraySquare() {

  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, tex );    


  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);

  glColorPointer(4, GL_FLOAT, 0, colors);
  glTexCoordPointer(3, GL_FLOAT, 0, texcoords);
  glVertexPointer(3, GL_FLOAT, 0, vertices);

  glDrawArrays(GL_QUADS, 0, 12);

  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

}


void immediateSquare() {
  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, tex );    


  glColor4f(1.0,1.0,1.0,1.0);

  glBegin( GL_QUADS );

  glTexCoord2f(0, 0);
  glVertex3f(-.5, -.5, 0);
  glTexCoord2f(1, 0);
  glVertex3f(.5, -.5, 0);
  glTexCoord2f(1, 1);
  glVertex3f(.5, .5, 0);
  glTexCoord2f(0, 1);
  glVertex3f(-.5, .5, 0);

  glEnd();
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
     SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y //| SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
     //0
    );

  /* check for an error during the load process */
  if( 0 == tex )
  {
    printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
  }
}

void initVertexArrays() {
    }

void init(void) {

  glEnable(GL_DEPTH_TEST);

  glClearColor(0.0,0.0,0.0,0.0);

  qObj = gluNewQuadric(); //load up new quadric 
  //gluQuadricDrawStyle(qObj, GLU_FILL); //fill in the facets on the surface 
  gluQuadricNormals(qObj, GLU_SMOOTH); //make nice normals across the facets
  //if lighting is enabled then the normals are already present
  gluQuadricTexture(qObj, GL_TRUE); //automatically create texture coords for the object
  //if you a bind a texture, then the texture coords are already present

  initVertexArrays();

  loadTextures();

}


void keyboard(unsigned char key, int x, int y){
    printf("you pressed %c \n", key);
  
    switch(key) {
      case 'p':
	break;
      case 'q':
	break;
      }
}

void keySpecial (int key, int x, int y) {  

  //printf("special key pressed:< %c > \n", key);

  if (key==GLUT_KEY_LEFT) {
    printf("LEFT \n");
  } else if (key==GLUT_KEY_RIGHT) {
    printf("RIGHT \n");
  } else if (key==GLUT_KEY_UP) {
    printf("UP \n");
  } else if (key==GLUT_KEY_DOWN) {
    printf("DOWN \n");
  } else if (key==GLUT_KEY_HOME) {
    printf("HOME \n");
  } else if (key==GLUT_KEY_END) {
    printf("END \n");
  } else if (key==GLUT_KEY_PAGE_UP) {
    printf("PAGE_UP \n");
  } else if (key==GLUT_KEY_PAGE_DOWN) {
    printf("PAGE_DOWN \n");
  }
}


void timer(int value){
//  // get new color or a value in [0,1]
//	G_rot+=PI/4.0;

  printf("CLICK 1... \n");
  glutTimerFunc(1000,timer,0);
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
  //glutTimerFunc(1000/60,timer,0);


  glutMainLoop();
  return EXIT_SUCCESS;
}


