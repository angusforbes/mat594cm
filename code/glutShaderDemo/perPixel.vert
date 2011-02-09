varying vec3 normal;
varying vec4 pos;

void main() {
  normal = gl_NormalMatrix * gl_Normal; //the normal in eye coordinates
  pos = gl_ModelViewMatrix * gl_Vertex; //the position in eye coordinates
  gl_Position = ftransform(); //the position in clip coordinates

}

