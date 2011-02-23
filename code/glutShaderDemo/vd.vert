varying vec3 normal;
varying vec4 pos;
attribute float wave; //frame number

void main() {
  normal = gl_NormalMatrix * gl_Normal; //the normal in eye coordinates

  vec4 vertex = gl_Vertex;
  vertex.y = vertex.y + sin(vertex.x + wave);
  pos = gl_ModelViewMatrix * vertex; //the displaced position in eye coordinates
 
  gl_Position = gl_ModelViewProjectionMatrix * vertex; //the displaced position in clip coordinates


  //gl_Position = ftransform(); //the position in clip coordinates

}

