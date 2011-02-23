#ifdef _WIN32
#include "windows.h"
#endif

#include "GLUT/glut.h"

#ifdef _WIN32
#include "glext.h"
#endif


#include <stdio.h>


#ifdef _WIN32
// As microsoft did not maintain openGL after version 1.1, Windows platform need to go throught this crap ; macosX and Linux are fine.
// This block simply retries openGL function needed for this example.
// I recommend to use GLEW instead of going this way. This is done this way only to ease beginner's compilation and portability


PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;

// FrameBuffer (FBO) gen, bin and texturebind
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT ;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT ;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT ;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT ;


// Shader functions
PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB ;
PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB   ;
PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB ;
PFNGLSHADERSOURCEARBPROC         glShaderSourceARB        ;
PFNGLCOMPILESHADERARBPROC        glCompileShaderARB       ;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
PFNGLATTACHOBJECTARBPROC         glAttachObjectARB        ;
PFNGLLINKPROGRAMARBPROC          glLinkProgramARB         ;
PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB  ;
PFNGLUNIFORM1IARBPROC            glUniform1iARB           ;
PFNGLUNIFORM1FARBPROC            glUniform1fARB           ;
PFNGLUNIFORM2FARBPROC			 glUniform2fARB            ;
PFNGLACTIVETEXTUREARBPROC		  glActiveTextureARB;
PFNGLGETINFOLOGARBPROC           glGetInfoLogARB          ;
PFNGLGENERATEMIPMAPEXTPROC		 glGenerateMipmapEXT;

void getOpenGLFunctionPointers(void)
{
	// FBO
	glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	glGenFramebuffersEXT		= (PFNGLGENFRAMEBUFFERSEXTPROC)		wglGetProcAddress("glGenFramebuffersEXT");
	glBindFramebufferEXT		= (PFNGLBINDFRAMEBUFFEREXTPROC)		wglGetProcAddress("glBindFramebufferEXT");
	glFramebufferTexture2DEXT	= (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
	glCheckFramebufferStatusEXT	= (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
	
	//Shaders
	glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
	glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
	glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
	glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB"); 
	glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB"); 
	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB"); 
	glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB"); 
	glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetInfoLogARB");
	glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
	glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
	glUniform1iARB = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
	glUniform1fARB = (PFNGLUNIFORM1FARBPROC)wglGetProcAddress("glUniform1fARB");
	// Use by blur shader
	glUniform2fARB = (PFNGLUNIFORM2FARBPROC)wglGetProcAddress("glUniform2fARB");

	// Used during color depth filtering 
	glGenerateMipmapEXT = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress("glGenerateMipmapEXT");
	
}
#endif

// Expressed as float so gluPerspective division returns a float and not 0 (640/480 != 640.0/480.0).
#define RENDER_WIDTH 1024.0
#define RENDER_HEIGHT 768.0
#define SHADOW_MAP_COEF 0.5
#define BLUR_COEF 0.25


//Camera position
float p_camera[4] = {35,25,5,1};

//Camera lookAt
float l_camera[3] = {0,0,-10};

//Light position
float p_light[4] = {3,18,0,1};

//Light lookAt
float l_light[3] = {0,0,-5};


//Light mouvement circle radius
float light_mvnt = 35.0f;

// Hold id of the framebuffer for light POV rendering
GLuint fboId;
// Z values will be rendered to this texture when using fboId framebuffer
GLuint depthTextureId;
GLuint colorTextureId;

// Use to activate/disable shadowShader
GLhandleARB shadowShaderId;
GLuint shadowMapUniform;
GLuint shadowMapStepXUniform;
GLuint shadowMapStepYUniform;

// Used to store values during the first pass
GLuint storeMomentsShader;

// Bluring FBO
GLuint blurFboId;
// Z values will be rendered to this texture when using fboId framebuffer
GLuint blurFboIdColorTextureId;

// Used to blur the depth values
GLuint blurShader;
GLuint scaleUniform; // Used to pass blur horiz or vert
GLuint textureSourceUniform;

// Loading shader function
GLhandleARB loadShader(char* filename, unsigned int type)
{
	FILE *pfile;
	GLhandleARB handle;
	const GLcharARB* files[1];
	
	// shader Compilation variable
	GLint result;				// Compilation code result
	GLint errorLoglength ;
	char* errorLogText;
	GLsizei actualErrorLogLength;
	
	char buffer[400000];
	memset(buffer,0,400000);
	
	// This will raise a warning on MS compiler
	pfile = fopen(filename, "rb");
	if(!pfile)
	{
		printf("Sorry, can't open file: '%s'.\n", filename);
		exit(0);
	}
	
	fread(buffer,sizeof(char),400000,pfile);
	//printf("%s\n",buffer);
	
	
	fclose(pfile);
	
	handle = glCreateShaderObjectARB(type);
	if (!handle)
	{
		//We have failed creating the vertex shader object.
		printf("Failed creating vertex shader object from file: %s.",filename);
		exit(0);
	}
	
	files[0] = (const GLcharARB*)buffer;
	glShaderSourceARB(
					  handle, //The handle to our shader
					  1, //The number of files.
					  files, //An array of const char * data, which represents the source code of theshaders
					  NULL);
	
	glCompileShaderARB(handle);
	
	//Compilation checking.
	glGetObjectParameterivARB(handle, GL_OBJECT_COMPILE_STATUS_ARB, &result);
	
	// If an error was detected.
	if (!result)
	{
		//We failed to compile.
		printf("Shader '%s' failed compilation.\n",filename);
		
		//Attempt to get the length of our error log.
		glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &errorLoglength);
		
		//Create a buffer to read compilation error message
		errorLogText = malloc(sizeof(char) * errorLoglength);
		
		//Used to get the final length of the log.
		glGetInfoLogARB(handle, errorLoglength, &actualErrorLogLength, errorLogText);
		
		// Display errors.
		printf("%s\n",errorLogText);
		
		// Free the buffer malloced earlier
		free(errorLogText);
	}
	
	return handle;
}

void loadShadowShader()
{
	GLhandleARB vertexShaderHandle;
	GLhandleARB fragmentShaderHandle;
	

	vertexShaderHandle   = loadShader("VertexShader.c",GL_VERTEX_SHADER);
	fragmentShaderHandle = loadShader("FragmentShader.c",GL_FRAGMENT_SHADER);
	shadowShaderId = glCreateProgramObjectARB();
	glAttachObjectARB(shadowShaderId,vertexShaderHandle);
	glAttachObjectARB(shadowShaderId,fragmentShaderHandle);
	glLinkProgramARB(shadowShaderId);
	shadowMapUniform = glGetUniformLocationARB(shadowShaderId,"ShadowMap");
	shadowMapStepXUniform = glGetUniformLocationARB(shadowShaderId,"xPixelOffset");
	shadowMapStepYUniform = glGetUniformLocationARB(shadowShaderId,"yPixelOffset");


	vertexShaderHandle   = loadShader("StoreDepthVertexShader.c",GL_VERTEX_SHADER);
	fragmentShaderHandle = loadShader("StoreDepthFragmentShader.c",GL_FRAGMENT_SHADER);
	storeMomentsShader = glCreateProgramObjectARB();
	glAttachObjectARB(storeMomentsShader,vertexShaderHandle);
	glAttachObjectARB(storeMomentsShader,fragmentShaderHandle);
	glLinkProgramARB(storeMomentsShader);

	vertexShaderHandle   = loadShader("BlurVertexShader.c",GL_VERTEX_SHADER);
	fragmentShaderHandle = loadShader("BlurFragmentShader.c",GL_FRAGMENT_SHADER);
	blurShader = glCreateProgramObjectARB();
	glAttachObjectARB(blurShader,vertexShaderHandle);
	glAttachObjectARB(blurShader,fragmentShaderHandle);
	glLinkProgramARB(blurShader);
	scaleUniform = glGetUniformLocationARB(blurShader,"ScaleU");
	textureSourceUniform = glGetUniformLocationARB(blurShader,"textureSource");
	
}

void generateShadowFBO()
{
	int shadowMapWidth = RENDER_WIDTH * SHADOW_MAP_COEF;
	int shadowMapHeight = RENDER_HEIGHT * SHADOW_MAP_COEF;
	
	//GLfloat borderColor[4] = {0,0,0,0};
	
	GLenum FBOstatus;
	
	// Try to use a texture depth component
	glGenTextures(1, &depthTextureId);
	glBindTexture(GL_TEXTURE_2D, depthTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	// Remove artefact on the edges of the shadowmap
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	
	// No need to force GL_DEPTH_COMPONENT24, drivers usually give you the max precision if available 
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1,&colorTextureId);
	glBindTexture(GL_TEXTURE_2D, colorTextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	

	// Remove artefact on the edges of the shadowmap
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F_ARB, shadowMapWidth, shadowMapHeight, 0, GL_RGB, GL_FLOAT, 0);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);



	// create a framebuffer object
	glGenFramebuffersEXT(1, &fboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
	
	// attach the texture to FBO depth attachment point
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT ,GL_TEXTURE_2D, depthTextureId, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D, colorTextureId, 0);

	// check FBO status
	FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed for shadowmap FBO, CANNOT use FBO\n");
	
	// switch back to window-system-provided framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);



	// Creating the blur FBO
	glGenFramebuffersEXT(1, &blurFboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, blurFboId);

	glGenTextures(1,&blurFboIdColorTextureId);
	glBindTexture(GL_TEXTURE_2D, blurFboIdColorTextureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F_ARB, shadowMapWidth*BLUR_COEF, shadowMapHeight*BLUR_COEF, 0, GL_RGB, GL_FLOAT, 0);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D, blurFboIdColorTextureId, 0);
	FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
		printf("GL_FRAMEBUFFER_COMPLETE_EXT failed for blur FBO, CANNOT use FBO\n");

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void setupMatrices(float position_x,float position_y,float position_z,float lookAt_x,float lookAt_y,float lookAt_z)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,RENDER_WIDTH/RENDER_HEIGHT,10,120);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(position_x,position_y,position_z,lookAt_x,lookAt_y,lookAt_z,0,1,0);
}


// This update only change the position of the light.
//int elapsedTimeCounter = 0;
void update(void)
{
	//printf("%d\n",glutGet(GLUT_ELAPSED_TIME));
	p_light[0] = light_mvnt * cos(glutGet(GLUT_ELAPSED_TIME)/2000.0);
	p_light[2] = light_mvnt * sin(glutGet(GLUT_ELAPSED_TIME)/2000.0);
	
	//p_light[0] = light_mvnt * cos(4000/1000.0);
	//p_light[2] = light_mvnt * sin(4000/1000.0);

	//p_light[0] = light_mvnt * cos(3000/1000.0);
	//p_light[2] = light_mvnt * sin(3000/1000.0);


}


void setTextureMatrix(void)
{
	static double modelView[16];
	static double projection[16];
	
	// This is matrix transform every coordinate x,y,z
	// x = x* 0.5 + 0.5 
	// y = y* 0.5 + 0.5 
	// z = z* 0.5 + 0.5 
	// Moving from unit cube [-1,1] to [0,1]  
	const GLdouble bias[16] = {	
		0.5, 0.0, 0.0, 0.0, 
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0};
	
	// Grab modelview and transformation matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	
	
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	
	glLoadIdentity();	
	glLoadMatrixd(bias);
	
	// concatating all matrice into one.
	glMultMatrixd (projection);
	glMultMatrixd (modelView);
	
	// Go back to normal matrix mode
	glMatrixMode(GL_MODELVIEW);
}

// During translation, we also have to maintain the GL_TEXTURE8, used in the shadow shader
// to determine if a vertex is in the shadow.
void startTranslate(float x,float y,float z)
{
	glPushMatrix();
	glTranslatef(x,y,z);
	
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE7);
	glPushMatrix();
	glTranslatef(x,y,z);
}

void endTranslate()
{
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void drawObjects(void)
{
	// Ground
	glColor4f(0.3f,0.3f,0.3f,1);
	glBegin(GL_QUADS);
	glVertex3f(-45,2,-45);
	glVertex3f(-45,2, 55);
	glVertex3f( 55,2, 55);
	glVertex3f( 55,2,-45);
	glEnd();
	
	glColor4f(0.9f,0.9f,0.9f,1);
	
	// Instead of calling glTranslatef, we need a custom function that also maintain the light matrix
	startTranslate(0,8,-16);
	glutSolidCube(4);
	endTranslate();

	startTranslate(-8,4,-16);
	glutSolidCube(4);
	endTranslate();

	startTranslate(8,4,-16);
	glutSolidCube(4);
	endTranslate();
	
	startTranslate(0,8,-5);
	//glutSolidCube(4);
	glutSolidSphere(4,40,40);
	endTranslate();
	
	
}

void blurShadowMap(void)
{
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	
	// Bluring the shadow map  horinzontaly
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,blurFboId);	
		glViewport(0,0,RENDER_WIDTH * SHADOW_MAP_COEF *BLUR_COEF ,RENDER_HEIGHT* SHADOW_MAP_COEF*BLUR_COEF);
		glUseProgramObjectARB(blurShader);
		glUniform2fARB(scaleUniform,1.0/ (RENDER_WIDTH * SHADOW_MAP_COEF * BLUR_COEF),0.0);		// Bluring horinzontaly
		glUniform1iARB(textureSourceUniform,0);
		glActiveTextureARB(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,colorTextureId);

		//Preparing to draw quad
		 glMatrixMode(GL_PROJECTION);
		 glLoadIdentity();
		 glOrtho(-RENDER_WIDTH/2,RENDER_WIDTH/2,-RENDER_HEIGHT/2,RENDER_HEIGHT/2,1,20);
		 glMatrixMode(GL_MODELVIEW);
		 glLoadIdentity();

		 //Drawing quad 
		 glTranslated(0,0,-5);
		 glBegin(GL_QUADS);
			 glTexCoord2d(0,0);glVertex3f(-RENDER_WIDTH/2,-RENDER_HEIGHT/2,0);
			 glTexCoord2d(1,0);glVertex3f(RENDER_WIDTH/2,-RENDER_HEIGHT/2,0);
			 glTexCoord2d(1,1);glVertex3f(RENDER_WIDTH/2,RENDER_HEIGHT/2,0);
			 glTexCoord2d(0,1);glVertex3f(-RENDER_WIDTH/2,RENDER_HEIGHT/2,0);
		 glEnd();
		//glGenerateMipmapEXT(GL_TEXTURE_2D);
		
		 
		 // Bluring vertically
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fboId);	
		glViewport(0,0,RENDER_WIDTH * SHADOW_MAP_COEF ,RENDER_HEIGHT* SHADOW_MAP_COEF);
		glUniform2fARB(scaleUniform,0.0, 1.0/ (RENDER_HEIGHT * SHADOW_MAP_COEF ) );		
		glBindTexture(GL_TEXTURE_2D,blurFboIdColorTextureId);
		glBegin(GL_QUADS);
			 glTexCoord2d(0,0);glVertex3f(-RENDER_WIDTH/2,-RENDER_HEIGHT/2,0);
			 glTexCoord2d(1,0);glVertex3f(RENDER_WIDTH/2,-RENDER_HEIGHT/2,0);
			 glTexCoord2d(1,1);glVertex3f(RENDER_WIDTH/2,RENDER_HEIGHT/2,0);
			 glTexCoord2d(0,1);glVertex3f(-RENDER_WIDTH/2,RENDER_HEIGHT/2,0);
		glEnd();
		
}

void renderScene(void) 
{
	update();
	
	//First step: Render from the light POV to a FBO, store depth and square depth in a 32F frameBuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,fboId);	//Rendering offscreen
	
	//Using the custom shader to do so
	glUseProgramObjectARB(storeMomentsShader);
	
	// In the case we render the shadowmap to a higher resolution, the viewport must be modified accordingly.
	glViewport(0,0,RENDER_WIDTH * SHADOW_MAP_COEF,RENDER_HEIGHT* SHADOW_MAP_COEF);
	
	// Clear previous frame values
	glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
		
	setupMatrices(p_light[0],p_light[1],p_light[2],l_light[0],l_light[1],l_light[2]);
	
	// Culling switching, rendering only backface, this is done to avoid self-shadowing
	//glCullFace(GL_FRONT);
	drawObjects();
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	//Save modelview/projection matrice into texture7, also add a biais
	setTextureMatrix();
	
	blurShadowMap();

	// DEBUG only. this piece of code draw the depth buffer onscreen
	/*
	 glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	 glUseProgramObjectARB(0);
	 glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
	 glOrtho(-RENDER_WIDTH/2,RENDER_WIDTH/2,-RENDER_HEIGHT/2,RENDER_HEIGHT/2,1,20);
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
	 glColor4f(1,1,1,1);
	 glActiveTextureARB(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D,blurFboIdColorTextureId);
	 glEnable(GL_TEXTURE_2D);
	 glTranslated(0,0,-1);
	 glBegin(GL_QUADS);
	 glTexCoord2d(0,0);glVertex3f(0,0,0);
	 glTexCoord2d(1,0);glVertex3f(RENDER_WIDTH/2,0,0);
	 glTexCoord2d(1,1);glVertex3f(RENDER_WIDTH/2,RENDER_HEIGHT/2,0);
	 glTexCoord2d(0,1);glVertex3f(0,RENDER_HEIGHT/2,0);
	 glEnd();
	 glDisable(GL_TEXTURE_2D);
	 
	glutSwapBuffers();
	return;
	*/

	// Now rendering from the camera POV, using the FBO to generate shadows
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
	
	glViewport(0,0,RENDER_WIDTH,RENDER_HEIGHT);
		
	// Clear previous frame values
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Using the shadow shader
	glUseProgramObjectARB(shadowShaderId);
	glUniform1iARB(shadowMapUniform,7);
	glUniform1fARB(shadowMapStepXUniform,1.0/ (RENDER_WIDTH * SHADOW_MAP_COEF));
	glUniform1fARB(shadowMapStepYUniform,1.0/ (RENDER_HEIGHT * SHADOW_MAP_COEF));
	glActiveTextureARB(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D,colorTextureId);
	
	
	
		

	
	setupMatrices(p_camera[0],p_camera[1],p_camera[2],l_camera[0],l_camera[1],l_camera[2]);
	
	glLightfv(GL_LIGHT0, GL_POSITION, p_light);

	glCullFace(GL_BACK);
	drawObjects();
	
	// DEBUG only. this piece of code draw the depth buffer onscreen
	/*
	 glUseProgramObjectARB(0);
	 glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
	 glOrtho(-RENDER_WIDTH/2,RENDER_WIDTH/2,-RENDER_HEIGHT/2,RENDER_HEIGHT/2,1,20);
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
	 glColor4f(1,1,1,1);
	 glActiveTextureARB(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D,blurFboIdColorTextureId);
	 glEnable(GL_TEXTURE_2D);
	 glTranslated(0,0,-1);
	 glBegin(GL_QUADS);
	 glTexCoord2d(0,0);glVertex3f(0,0,0);
	 glTexCoord2d(1,0);glVertex3f(RENDER_WIDTH/2,0,0);
	 glTexCoord2d(1,1);glVertex3f(RENDER_WIDTH/2,RENDER_HEIGHT/2,0);
	 glTexCoord2d(0,1);glVertex3f(0,RENDER_HEIGHT/2,0);
	 glEnd();
	 glDisable(GL_TEXTURE_2D);
	 */
	
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {
	
	if (key == 27) 
		exit(0);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(RENDER_WIDTH,RENDER_HEIGHT);
	glutCreateWindow("GLSL Shadow mapping");
	
	printf("Shadow Map: %.0fx%.0f\n",RENDER_WIDTH*SHADOW_MAP_COEF,RENDER_HEIGHT*SHADOW_MAP_COEF);
	printf("Ping-Pong FBO: %.0fx%.0f\n",RENDER_WIDTH*SHADOW_MAP_COEF*BLUR_COEF,RENDER_HEIGHT*SHADOW_MAP_COEF*BLUR_COEF);

	// This call will grab openGL extension function pointers.
	// This is not necessary for macosx and linux
#ifdef _WIN32
	getOpenGLFunctionPointers();
#endif
	generateShadowFBO();
	loadShadowShader();
	
	// This is important, if not here, FBO's depthbuffer won't be populated.
	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1.0f);
	
	glEnable(GL_CULL_FACE);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	
	
	
	
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	
	glutKeyboardFunc(processNormalKeys);
	
	glutMainLoop();
}
