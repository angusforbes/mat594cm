
import processing.opengl.*;
import javax.media.opengl.*; 
import javax.media.opengl.glu.*;
import com.sun.opengl.util.texture.*; 

float lightX = 20f;
float lightY = 20f;
float lightZ = 20f;

float camX = 0f;
float camY = 0f;
float camZ = -10f;
float rotCamX = 0f;
float rotCamY = 0f;
float rotCamZ = 0f;

float objX = 0f;
float objY = 0f;
float objZ = 0f;
float rotObjX = 0f;
float rotObjY = 0f;
float rotObjZ = 0f;

float translateInc = 1f;

float FOV = 15f;
float ASPECT_RATIO = 0f;
float NEAR = 1f;
float FAR = 100f;

PFont f, f2;

//PGraphicsOpenGL pgl;
PGraphics3D pgl;

GLU glu;
float[] viewMatrix = new float[16];
float[] objectMatrix = new float[16];
float[] modelviewMatrix = new float[16];
float[] projectionMatrix = new float[16];
float x = -0f;
float y = -0f;
float w = 80f;
float h = 80f;
float[] objectPt = new float[]{ w/2, h/2, 0f, 1f };
//float[] objectPt = new float[]{ 0f, 0f, 0f, 0f };

float[] modelviewPt = new float[4];
float[] clipPt = new float[4];
float[] devicePt = new float[4];
float[] windowPt = new float[4];

int[] viewportBounds = new int[4];
NumberFormat decimalFormatter = new DecimalFormat("##.##");
NumberFormat decimalFormatter2 = new DecimalFormat("##.##");

void setup() 
{
  size(860, 500, OPENGL); 

  viewportBounds = new int[]{ 
    0,0, width, height     };

  background(0);

  f = loadFont("LucidaConsole-9.vlw");
  f2 = loadFont("LucidaConsole-12.vlw");
}

void displayVecData(String name, float[] vec, int pmx, int pmy, int pmw, int pmh)
{
  fill(0,0,128,255);
  rect(pmx, pmy, pmw, pmh);

  fill(255);
  textAlign(LEFT);
  textFont(f2, 12);

  text(name, pmx, pmy);
  textFont(f, 9);

  for (int i = 0 ; i < vec.length; i++)
  {
    text(decimalFormatter2.format(vec[i]), pmx + 3, pmy + ((pmh / 4) * i ) + 15 );
  }
}


void displayMatrixData(String name, float[] matrix, int pmx, int pmy, int pmw, int pmh)
{
  fill(0,128,0,255);
  rect(pmx, pmy, pmw, pmh);

  fill(255);
  textAlign(LEFT);
  textFont(f2, 12);

  text(name, pmx, pmy);
  textFont(f, 9);

  for (int i = 0 ; i < matrix.length; i++)
  {
    int cx = i % 4;
    int cy = i / 4;
    text(decimalFormatter.format(matrix[i]), pmx + ((pmw / 4) * cx) + 5, pmy + ((pmh / 4) * cy ) + 15 );
  }
}

void setObjectMatrix()
{
  pushMatrix();
  {
    resetMatrix();
    transformObject();
    objectMatrix = pgl.modelview.get(objectMatrix);
  }
  popMatrix();
}

void transformObject()
{
  //center the object in the center of screen 
  translate(width/2 - w/2, height/2 - h/2, 0);

  //translate object
  translate(objX, objY, objZ);

  //rotate around center of object
  translate(w/2, h/2, 0);
  rotateX(radians(rotObjX));
  rotateY(radians(rotObjY));
  rotateZ(radians(rotObjZ));
  translate(-w/2, -h/2, 0);
}

void drawCoordinates()
{
  stroke(0); //set border for matrix panels
  
  int hPx = 75;
  int yPx = height - hPx;

  displayVecData("obj", objectPt,  0, yPx, 40, hPx);
  displayMatrixData("model matrix", objectMatrix, 50, yPx, 150, hPx);
  displayMatrixData("view matrix", viewMatrix, 200, yPx, 150, hPx);
  displayMatrixData("modelview matrix", modelviewMatrix, 350, yPx, 150, hPx);

  //float[] hPt = new float[]{objectPt[0], objectPt[1], objectPt[2], 0f}; 
  modelviewPt = MatrixUtils.multiplyMatrixByVector(modelviewMatrix, objectPt);
  displayVecData("eye", modelviewPt,  510, yPx, 40, hPx);

  displayMatrixData("projection matrix", projectionMatrix, 560, yPx, 150, hPx);
  clipPt = MatrixUtils.multiplyMatrixByVector(projectionMatrix, modelviewPt);

  displayVecData("clip", clipPt,  720, yPx, 40, hPx);
  devicePt = MatrixUtils.normalizeHomogeneousVector(clipPt);
  displayVecData("device", devicePt,  770, yPx, 40, hPx);
  
  windowPt = MatrixUtils.deviceCoordsToWindowCoords(devicePt, viewportBounds);
  displayVecData("pixel", windowPt,  820, yPx, 40, hPx);
} 

void draw()
{
  camera(); //reset camera
  background(0); //clear screen
  pgl = (PGraphics3D) g; //keep track of graphics object so we can get the matrices
 
  drawCoordinates(); 
  
  setObjectMatrix(); //calculate and store object matrix

  beginCamera();
  {
    perspective();

    projectionMatrix = pgl.projection.get(projectionMatrix); //store projection matrix

    rotateX(radians(rotCamX));
    rotateY(radians(rotCamY));
    rotateZ(radians(rotCamZ));
    translate(camX, camY, camZ);
  }
  endCamera();
  
  viewMatrix = pgl.modelview.get(viewMatrix); //store view matrix

  pushMatrix();
  {
    transformObject();
    
    modelviewMatrix = pgl.modelview.get(modelviewMatrix); //store modelview matrix
    
    
    //draw object
    beginShape(LINES);
    stroke(255,0,0);
    
    vertex(x, y, 0.0);
    vertex(x + w, y, 0.0);
    
    stroke(0,255,0);
    vertex(x, y, 0.0);
    vertex(x, y + h, 0.0);
    
    stroke(0,0,255);
    vertex(x, y, 0.0);
    vertex(x, y, w);
    endShape();
    
    textFont(f2);
    text("+x", w, 0, 0);
    text("+y", 0, h, 0);
    text("+z", 0, 0, w);
    
    //draw object point (the center of the square)
    fill(0,0,255);
    beginShape(QUADS);
    vertex(x + w/2 - 4, y + h/2 - 4, 1.0);
    vertex(x + w/2 + 4, y + h/2 - 4, 1.0);
    vertex(x + w/2 + 4, y + h/2 + 4, 1.0);
    vertex(x + w/2 - 4, y + h/2 + 4, 1.0);
    endShape();

  }
  popMatrix();


  //draw dot at origin of world coordinates
  pushMatrix();
  {
    fill(0,255,0);
  
    beginShape(QUADS);
    vertex(width/2 - 2, height/2 - 2, 0);
    vertex(width/2 + 2, height/2 - 2, 0);
    vertex(width/2 + 2, height/2 + 2, 0);
    vertex(width/2 -2, height/2 + 2, 0);
    endShape();
  }
  popMatrix();

}


void keyPressed()
{
  switch(key)
  {
    //change FOV
  case '[':
    FOV += 1f;
    break;
  case ']':
    FOV -= 1f;
    break;

    //translate lights
  case 'a':
    lightX += 5;
    break;
  case 'z':
    lightX -= 5;
    break;
  case 's':
    lightY -= 5;
    break;
  case 'x':
    lightY += 5;
    break;
  case 'd':
    lightZ += 5;
    break;
  case 'c':
    lightZ -= 5;
    break;

    //translate object
  case '1':
    objX -= translateInc;
    break;
  case 'q':
    objX += translateInc;
    break;
  case '2':
    objY -= translateInc;
    break;
  case 'w':
    objY += translateInc;
    break;
  case '3':
    objZ -= translateInc;
    break;
  case 'e':
    objZ += translateInc;
    break;


    //rotate obj
  case '4':
    rotObjX -= 1f;
    break;
  case 'r':
    rotObjX += 1f;
    break;
  case '5':
    rotObjY -= 1f;
    break;
  case 't':
    rotObjY += 1f;
    break;
  case '6':
    rotObjZ -= 1f;
    break;
  case 'y':
    rotObjZ += 1f;
    break;

    //rotate camera
  case '8':
    rotCamX -= 1f;
    break;
  case 'i':
    rotCamX += 1f;
    break;
  case '9':
    rotCamY -= 1f;
    break;
  case 'o':
    rotCamY += 1f;
    break;
  case '0':
    rotCamZ -= 1f;
    break;
  case 'p':
    rotCamZ += 1f;
    break;

    //translate camera
  case 'h':
    camX -= .1f;
    break;
  case 'n':
    camX += .1f;
    break;
  case 'j':
    camY -= .1f;
    break;
  case 'm':
    camY += .1f;
    break;
  case 'k':
    camZ -= .1f;
    break;
  case ',':
    camZ += .1f;
    break;

  case ' ':
    reset();
    break;  
  }
}


void mouseDragged() 
{
  float rate = 5f;
  rotObjX += (pmouseY-mouseY) * rate;
  rotObjY += (mouseX-pmouseX) * rate;
}

void reset()
{ 
  lightX = width/2f + 30f;
  lightY = height/2f + 30f;
  lightZ = 100f;
  rotObjX = 0f;
  rotObjY = -30f;
  rotObjZ = 0f;
  rotCamX = 0f;
  rotCamY = 0f;
  rotCamZ = 0f;
  camX = 0f;
  camY = 0f;
  camZ = 0f;
  objX = 0f;
  objY = 0f;
  objZ = 0f;

}









